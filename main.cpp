#include "pclviewer.h"
#include "KinectStream.h"
#include <QApplication>
#include <QMainWindow>

#include <thread>

PCLViewer* w;
void run(KinectStream s) {
	
	while (true) {
		Output ot = s.Tick(2);

		if (ot.count == 0) continue;
		PointCloudT::Ptr cloud;

		// Setup the cloud pointer
		cloud.reset(new PointCloudT);
		// The number of points in the cloud
		cloud->points.resize(ot.count);
		
		
		for (size_t i = 0; i < ot.count; i++)
		{
			CameraSpacePoint point = ot.csp[i];
			cloud->points[i].x = point.X;
			cloud->points[i].y = point.Y;
			cloud->points[i].z = point.Z;

			cloud->points[i].r = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
			cloud->points[i].g = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
			cloud->points[i].b = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
		}

		w->updateCloud(cloud);
	}
	//ICP icp;
	//icp.executeICP();

	return;

}


int main (int argc, char *argv[])
{
  QApplication a (argc, argv);
  w = new PCLViewer();
  KinectStream kinectStream;
  kinectStream.initKinect();
  
  std::thread t1(run, kinectStream);
  t1.detach();

  w->show ();

  return a.exec ();
}
