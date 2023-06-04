#include <random>
#include <opencv2/core/core.hpp>
#include "../Robot.h"
#include "../map/Map.h"

struct Pose {
    float x;
    float y;
    float theta;
};

struct Particle {
    Pose pose; // Posición de la partícula (x, y, theta)
    double weight;    // Peso de la partícula


    cv::Mat position();
    cv::Mat orientation();
};


class MonteCarloLocalization
{
private:
    int n_particles;
    std::vector<Particle> particles;
    std::normal_distribution<double> random_deviation;
    std::mt19937 deviation_generator;
    std::normal_distribution<double> random_rotation;
    std::mt19937 rotation_generator;


    Particle generate_random_particle(float x, float y, float theta);
    void generate_initial_particles(float x, float y, float theta);
    void write_particles(std::ostream& out);

    void update_particle_weights(const std::vector<cv::Point3f>& camera_points, Map& mapa);
    void resampleParticles();
    cv::Point2f estimateRobotPosition();

public:
    MonteCarloLocalization(int n_particles);

    void localize(Robot& robot, std::vector<cv::Point3f>& camera_points, Map& mapa);
};

