#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

struct Particle {
    cv::Point3f pose; // Posición de la partícula (x, y, theta)
    double weight;    // Peso de la partícula
};

int numParticles = 100; // Número de partículas
std::vector<Particle> particles(numParticles); // Vector para almacenar las partículas

// Genera un valor aleatorio dentro de un rango específico
double getRandomValue(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Genera una partícula con valores aleatorios para x, y y theta
Particle generateRandomParticle(double minX, double maxX, double minY, double maxY) {
    Particle particle;
    particle.pose.x = getRandomValue(minX, maxX);
    particle.pose.y = getRandomValue(minY, maxY);
    particle.pose.z = getRandomValue(0.0, 0.1); // Rango de rotación en el eje z
    return particle;
}

// Genera el conjunto inicial de partículas aleatorias
void generateInitialParticles(double minX, double maxX, double minY, double maxY) {
    for (int i = 0; i < numParticles; i++) {
        particles[i] = generateRandomParticle(minX, maxX, minY, maxY);
    }
}

void printParticles(const std::vector<Particle>& particles) {
    for (const auto& particle : particles) {
        std::cout << "Particle: x = " << particle.pose.x << ", y = " << particle.pose.y << ", theta = " << particle.pose.z << ", weight = " << particle.weight << std::endl;
    }
}

void updateParticleWeights(std::vector<Particle>& particles, const std::vector<cv::Point3f>& cameraPoints) {
    // Preparar los datos para el modelo KNN
    cv::Mat trainingData(cameraPoints.size(), 3, CV_32F);
    cv::Mat labels(cameraPoints.size(), 1, CV_32S);

    // Llenar los datos de entrenamiento
    for (size_t i = 0; i < cameraPoints.size(); i++) {
        trainingData.at<float>(i, 0) = cameraPoints[i].x;
        trainingData.at<float>(i, 1) = cameraPoints[i].y;
        trainingData.at<float>(i, 2) = cameraPoints[i].z;

        // Supongamos que si el punto es un obstáculo, se etiqueta como 1; de lo contrario, se etiqueta como 0
        labels.at<int>(i, 0) = (cameraPoints[i].z > 0.0) ? 1 : 0;
    }

    // Entrenar el modelo KNN
    int k = 5; // Número de vecinos más cercanos
    cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();
    knn->train(trainingData, cv::ml::ROW_SAMPLE, labels);

    // Iterar sobre las partículas y ajustar sus pesos utilizando KNN
    for (auto& particle : particles) {
        // Construir el vector de características para la partícula
        cv::Mat sample(1, 3, CV_32F);
        sample.at<float>(0, 0) = particle.pose.x;
        sample.at<float>(0, 1) = particle.pose.y;
        sample.at<float>(0, 2) = particle.pose.z;

        // Encontrar los vecinos más cercanos utilizando KNN
        cv::Mat neighborIndices, neighborDistances;
        knn->findNearest(sample, k, neighborIndices, neighborDistances);

        // Calcular el peso de la partícula basado en los vecinos más cercanos
        double weight = 0.0;
        for (int i = 0; i < neighborIndices.cols; i++) {
            int label = labels.at<int>(neighborIndices.at<float>(0, i), 0);
            if (label == 1) {
                weight += 1.0; // Si el vecino más cercano es un obstáculo, incrementar el peso
            }
        }

        particle.weight = weight;
    }
}

void resampleParticles(std::vector<Particle>& particles) {
    // Calcular la suma de los pesos de las partículas
    double sumWeights = 0.0;
    for (const auto& particle : particles) {
        sumWeights += particle.weight;
    }

    // Crear un vector auxiliar para almacenar las nuevas partículas seleccionadas
    std::vector<Particle> newParticles;

    // Realizar el re-muestreo
    for (int i = 0; i < numParticles; i++) {
        // Generar un número aleatorio entre 0 y la suma de los pesos
        double randomNum = getRandomValue(0.0, sumWeights);

        // Seleccionar una partícula utilizando el método de la ruleta
        double cumulativeWeight = 0.0;
        for (const auto& particle : particles) {
            cumulativeWeight += particle.weight;
            if (randomNum <= cumulativeWeight) {
                // Seleccionar esta partícula
                newParticles.push_back(particle);
                break;
            }
        }
    }
    // Actualizar el vector de partículas con las nuevas partículas seleccionadas
    particles = newParticles;
}

void moveParticles(std::vector<Particle>& particles, double deltaX, double deltaY, double deltaTheta) {
    // Aplicar el movimiento a cada partícula
    for (auto& particle : particles) {
        // Actualizar la posición
        particle.x += deltaX;
        particle.y += deltaY;
        // Actualizar la orientación
        particle.theta += deltaTheta;
    }
}

cv::Point2f estimateRobotPosition(const std::vector<Particle>& particles) {
    double totalWeight = 0.0;
    double sumX = 0.0;
    double sumY = 0.0;

    // Calcular la suma ponderada de las posiciones de las partículas
    for (const auto& particle : particles) {
        double weight = particle.weight;
        sumX += particle.x * weight;
        sumY += particle.y * weight;
        totalWeight += weight;
    }

    // Calcular la media ponderada de las posiciones
    double meanX = sumX / totalWeight;
    double meanY = sumY / totalWeight;

    return cv::Point2f(meanX, meanY);
}

int main() {
    // Configuración inicial
    srand(time(0)); // Inicializa la semilla del generador de números aleatorios

    // Posición estimada del robot
    cv::Point2f estimatedPosition(0.0, 0.0);

    // Establece los límites basados en la posición estimada del robot
    double minX = estimatedPosition.x - 0.05;
    double maxX = estimatedPosition.x + 0.05;
    double minY = estimatedPosition.y - 0.05;
    double maxY = estimatedPosition.y + 0.05;

    // Genera el conjunto inicial de partículas
    generateInitialParticles(minX, maxX, minY, maxY);

    // Puntos calculados de la cámara
    std::vector<cv::Point3f> cameraPoints = vision.detect_points(robot.compute_camera_position(), robot.get_orientation());
    // Agrega los puntos calculados de la cámara al vector cameraPoints
    // ...

    // Actualiza los pesos de las partículas utilizando los puntos de la cámara
    updateParticleWeights(particles, cameraPoints);

    // Imprime las partículas con sus pesos actualizados
    printParticles(particles);

    resampleParticles(particles);

    // Aplicar el movimiento del robot a las partículas
    double deltaX = ...; // Obtener el desplazamiento en el eje x
    double deltaY = ...; // Obtener el desplazamiento en el eje y
    double deltaTheta = ...; // Obtener el cambio en la orientación (rotación en el eje z)
    moveParticles(particles, deltaX, deltaY, deltaTheta);

    cv::Point2f robotPosition = estimateRobotPosition(particles);

    return 0;
}
