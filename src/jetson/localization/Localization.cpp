#include "Localization.h"
#include <opencv2/ml.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

cv::Mat Particle::position()
{
    cv::Mat pos = (cv::Mat_<float>(3, 1) <<
        pose.x,
        pose.y,
        0
    );

    return pos;
}

cv::Mat Particle::orientation()
{
    cv::Mat ori = (cv::Mat_<float>(3, 3) <<
        std::cos(pose.theta), -std::sin(pose.theta), 0,
        std::sin(pose.theta), std::cos(pose.theta), 0,
        0, 0, 1
    );

    return ori;
}




MonteCarloLocalization::MonteCarloLocalization(int n_particles):
    n_particles(n_particles),
    random_deviation(0.0, 0.1),
    deviation_generator(std::random_device()()),
    random_rotation(0.0, 0.261799),
    rotation_generator(std::random_device()())
{

}


Particle MonteCarloLocalization::generate_random_particle(float x, float y, float theta)
{
    Particle p;
    p.pose.x = x + random_deviation(deviation_generator);
    p.pose.y = y + random_deviation(deviation_generator);
    p.pose.theta = theta + random_rotation(rotation_generator);
    p.weight = 0;
    return p;
}

void MonteCarloLocalization::generate_initial_particles(float x, float y, float theta)
{
    particles.clear();
    particles.reserve(n_particles);
    for (int i = 0; i < n_particles; i++)
        particles.push_back(generate_random_particle(x, y, theta));
}


void MonteCarloLocalization::write_particles(std::ostream& out)
{
    for (const auto& particle : particles)
    {
        out << "Particle: x = " << particle.pose.x
            << ", y = " << particle.pose.y
            << ", theta = " << particle.pose.theta
            << ", weight = " << particle.weight << std::endl;
    }
}

void MonteCarloLocalization::update_particle_weights(const std::vector<cv::Point3f>& camera_points, Map& mapa)
{
    if (camera_points.size() == 0)
        throw std::runtime_error("Se ha intentado actualizar los pesos con camera_points.size() = 0.");
    std::cout << "camera_points.size() = " << camera_points.size() << std::endl;

    cv::Mat training_data(camera_points.size(), 3, CV_32F);
    cv::Mat labels(camera_points.size(), 1, CV_32S);

    // Llenar los datos de entrenamiento
    for (size_t i = 0; i < camera_points.size(); i++) {
        training_data.at<float>(i, 0) = camera_points[i].x;
        training_data.at<float>(i, 1) = camera_points[i].y;
        training_data.at<float>(i, 2) = camera_points[i].z;

        // Supongamos que si el punto es un obstáculo, se etiqueta como 1; de lo contrario, se etiqueta como 0
        labels.at<int>(i, 0) = (camera_points[i].z > 0.0) ? 1 : 0;
    }

    std::cout <<" Training data: " << training_data<< std::endl;

    // Entrenar el modelo KNN
    int k = 1; // Número de vecinos más cercanos
    cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();
    knn->train(training_data, cv::ml::ROW_SAMPLE, labels);


    // TODO: aqui es donde hay que hacer raycast desde cada particula
    // Iterar sobre las partículas y ajustar sus pesos utilizando KNN
    for (auto& particle : particles) {
        cv::Mat particle_cam_pos = particle.position();
        cv::Mat particle_cam_ori = particle.orientation();
	std::cout <<"Particle cam pos :" << particle_cam_pos << std::endl;
	std::cout <<"Particle cam ori : " << particle_cam_ori << std::endl << std::endl;
        std::vector<cv::Point3f> points = mapa.detect_points_with_virtual_camera(particle_cam_pos, particle_cam_ori);
        if (points.size() == 0)
        {
            particle.weight = 0;
            continue;
        }

        // TODO: revisar
        cv::Mat sample_data(points.size(), 3, CV_32F);

        for (size_t i = 0; i < points.size(); i++) {
            sample_data.at<float>(i, 0) = points[i].x;
            sample_data.at<float>(i, 1) = points[i].y;
            sample_data.at<float>(i, 2) = points[i].z;
        }

	std::cout << "FIND NEAREST SAMPLE DATA: " << sample_data << std::endl;

        // Encontrar los vecinos más cercanos utilizando KNN
        cv::Mat neighbor_indices, neighbor_labels, neighbor_distances;
        knn->findNearest(sample_data, k, neighbor_indices, neighbor_labels, neighbor_distances);

        // TODO: ajustar para que si hay pocos puntos no pueda dar bien

	std::cout<<"ASSURE NEIGHBOR: "<< neighbor_distances << std::endl;
        neighbor_distances = 1 / neighbor_distances;

        particle.weight = cv::sum(neighbor_distances)[0] / points.size();
    }
}

double getRandomValue(double min, double max){
    return min + ((double)rand() / RAND_MAX) * (max - min);
}


void MonteCarloLocalization::resampleParticles(){
	double sumWeights = 0.0;
	for (const auto&particle : particles) {
		sumWeights += particle.weight;
	}

	std::vector<Particle> newParticles;

	for (int i = 0; i < n_particles; i++) {
		double randomNum = getRandomValue(0.0, sumWeights);

		double cumulativeWeight = 0.0;
		for (const auto& particle : particles) {
			cumulativeWeight += particle.weight;
			if (randomNum <= cumulativeWeight) {
				newParticles.push_back(particle);
				break;
			}
		}
	}
	particles = newParticles;
}


cv::Point2f MonteCarloLocalization::estimateRobotPosition() {
    double totalWeight = 0.0;
    double sumX = 0.0;
    double sumY = 0.0;

    // Calcular la suma ponderada de las posiciones de las partículas
    for (const auto& particle : particles) {
	std::cout <<"PARTICLE VALUE: Particle weight: " << particle.weight << " , PoseX: " << particle.pose.x << " Particle pose Y: " << particle.pose.y << std::endl;
        double weight = particle.weight;
        sumX += particle.pose.x * weight;
        sumY += particle.pose.y * weight;
        totalWeight += weight;
    }

    // Calcular la media ponderada de las posiciones
    std::cout <<"ASSURANCE: sumX: " << sumX << ", sumY: " << sumY << ", totalWeight: "<< totalWeight << std::endl;
    double meanX = sumX / totalWeight;
    double meanY = sumY / totalWeight;

    return cv::Point2f(meanX, meanY);
}


void MonteCarloLocalization::localize(Robot& robot, std::vector<cv::Point3f>& camera_points, Map& mapa)
{
    float x = robot.get_position().at<float>(0);
    float y = robot.get_position().at<float>(1);
    float theta = std::atan2(
        robot.get_orientation().at<float>(1, 0),
        robot.get_orientation().at<float>(0, 0)
    );
    generate_initial_particles(x, y, theta);
    update_particle_weights(camera_points, mapa);
    resampleParticles();

    cv::Point2f estimatedPosit;
    estimatedPosit = estimateRobotPosition();
    std::cout << "Posición estimada:" <<  estimatedPosit << std::endl;
}
