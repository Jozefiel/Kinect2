#ifndef SUPPORT_H
#define SUPPORT_H

#include "camera.h"
#include "kinect.h"
#include "
#include "pcl.h"
#include "globalsettings.h"

#include <queue>
#include <experimental/filesystem>
#include <ctime>
#include <mutex>
#include <thread>

#include <QObject>
#include <QImage>
#include <QPixmap>


#define mutexTimeDelay 10
#define imshow_32to8 2048

namespace fs = std::experimental::filesystem;

class support : public QObject
{
    Q_OBJECT
public:
    support(QObject *parent = nullptr);

    void cameraInit();
    void kinectInit();
    void realsenseInit();
    int connectedCameras() { return static_cast<int>(connected_cams.size());}
    std::vector<Camera*> cameras();

    void threadsInit();
    void threadCameraSnapping();
    bool camera2framesDataTransfer();
    bool framesClouds2pclDataTransfer();

    void threadFrameUpdater();
    void closeThreads();

    void cloudInit();


    void viewerUpdater();
    void pclUpdater();
    void frameUpdater(std::atomic<bool> &snap_running);



    void transformCloud();
    void transformCloud(std::vector<Eigen::Matrix4d> transform_matrix);


    pcl::PointCloud<pcl::PointXYZRGB> getCloudData(int id);
    pcl::PointCloud<pcl::PointXYZRGB> getTransformedCloudData(int id);
    std::vector<std::shared_ptr<pclCloud>> getClouds();
    std::vector<pcl::PointCloud<pcl::PointXYZRGB>> mergeClouds(bool transformed);
    std::vector<pcl::PointCloud<pcl::PointXYZRGB>> mergeClouds(bool transformed, std::vector<Eigen::Matrix4d> transform_matrix);

    std::vector<Camera *> getConnectedCams();


    void changeComputeStyle(int);
    void saveLUT(cv::Mat depth, cv::Mat rgbd, cv::Mat ir, cv::Mat rgb, std::string filename, int counter);
    std::atomic<bool> snap_running {true};
    pcl::PointCloud<pcl::PointXYZRGB> cloudik;
    void saveSequence();

    void saveData();
    bool createDirectory(std::string path);

    template <class T>
    std::string IntToStr(T n)
    {
        std::stringstream result;
        result << n;
        return result.str();
    }


private:

    std::shared_ptr<GlobalSettings>         globalSettings = globalSettings->instance();
    std::vector<Camera*>                    connected_cams;                                            // vector of Camera objects
    std::vector<std::shared_ptr<pclCloud>>  clouds;
    std::vector<std::thread>                cam_threads;                                           // vector of threads for image snapping
    std::vector<std::thread>                cloud_threads;
    std::vector<std::thread>                viewer_threads;

    std::vector<std::vector<Camera::camera_frames>> * cam_frames=nullptr;

    std::atomic<bool> compute_cloud_style {false};
    std::vector<pcl::PointCloud<pcl::PointXYZRGB>> merged_clouds;

signals:
    void newRGBD(QPixmap pix,int id);
    void newDepth(QPixmap pix,int id);
    void newIR(QPixmap pix,int id);
    void newRangedRGBD(QPixmap pix,int id);
    void newRangedDepth(QPixmap pix,int id);
    void newHist(QPixmap pix,int id);
    void newCloud();

};



#endif // SUPPORT_H

