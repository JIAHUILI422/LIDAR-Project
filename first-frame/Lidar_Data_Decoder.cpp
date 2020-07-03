#include <aditof/camera.h>
#include <aditof/frame.h>
#include <aditof/system.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <functional>
#include <string.h>
#include <iostream>


using namespace aditof;

int main() {
	std::string fileName;
std::cin >> fileName;
std::ifstream playbackFile(fileName, std::ios::binary);
unsigned int height = 0;
unsigned int width = 0;
unsigned int fps = 0;

playbackFile.seekg(0, std::ios_base::end);
int fileSize = playbackFile.tellg();
playbackFile.seekg(0, std::ios_base::beg);

playbackFile.read(reinterpret_cast<char *>(&height), sizeof(int));
playbackFile.read(reinterpret_cast<char *>(&width), sizeof(int));
playbackFile.read(reinterpret_cast<char *>(&fps), sizeof(int));

int sizeOfHeader =3*  sizeof(int);
int sizeOfFrame = sizeof(uint16_t) * height * width;

// Find out how many frames you have in the file
int numberOfFrames = (fileSize - sizeOfHeader) / sizeOfFrame;

FrameDetails frameDetails;
frameDetails.height = height;
frameDetails.width = width;
std::shared_ptr<aditof::Frame> frame = 
	std::make_shared<aditof::Frame>();
// Set frame details
frame->setDetails(frameDetails);


uint16_t *frameDataLocation;
frame->getData(aditof::FrameDataType::RAW, &frameDataLocation);
frame->setDetails(frameDetails);
// Read one frame from the file. This read can be repeted 
// numberOfFrames times (playbackFile.eof() will return true)
int size = static_cast<int>(sizeof(uint16_t) * width * height);
std::cout << "Height " <<  height << " Width " << width << " FileSize " << fileSize << " Size "<< size << "\n";
std::cout <<"Number of Frames " <<numberOfFrames <<"\n";
std::shared_ptr<aditof::Frame> localFrame = 
	std::make_shared<aditof::Frame>();

for (int j = 0; j<numberOfFrames;j++){
	uint16_t *data;
	uint16_t *irdata;
playbackFile.read(reinterpret_cast<char *>(frameDataLocation),size);
frame->getData(aditof::FrameDataType::DEPTH,&data);
frame->getData(aditof::FrameDataType::IR,&irdata);
aditof::FrameDetails FDetail;
frame->getDetails(FDetail);

int FrameHeight = static_cast<int>(FDetail.height)/2;
int FrameWidth = static_cast<int>(FDetail.width);



//std::cout << "Depth of final pixel " << data[614399] <<"\n";
//std::cout << "Depth of final+1 pixel " << data[614400] <<"\n";
//std::cout << "-----------------------------\n";
std::ofstream DataFile;
std::stringstream FrameNumb_buf;
FrameNumb_buf <<std::setw(4) << std::setfill('0') << j+1;
std::string FrameNumb = FrameNumb_buf.str();
std::string Data_Name = "FrameData/Data_Frame_" + FrameNumb +".txt";
DataFile.open(Data_Name);
for( unsigned int i=0; i<width*height;i++)
{
	DataFile <<data[i]<<" ";
}
DataFile.close();



}
return 0;
}
