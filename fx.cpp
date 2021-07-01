/*
interacts with the library 
*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#define TERMINAL "ffmpeg -i "
// included headers

class fx
{
private:
    std::string fname;
    // holds filepath
public:
    fx(std::string &fname);
    // class constructor
    std::string getfname();
    // returns fname
    void setfname(std::string fname);
    // sets fname
    void exec(std::string &fname);
    // plays file
    void extractFrame(std::string &seektime, std::string &height, std::string &width, std::string &ofilepath);
    // performs extract frame operation
    void extractAudio(std::string &seektime, std::string &bitrate, std::string &ofilepath);
    // performs extract audio operation
    void extractClip(std::string &seektime, std::string &duration, std::string &ofilepath);
    // performs extract video operation
    void splitVideo(std::string &splitseek, std::string &ofilepath1, std::string &ofilepath2);
    // performs split video operation
    void overlayAudio(std::string &audiofile, std::string &ofilepath);
    // performs audio overlay operation
    void overlaySub(std::string &subfile, std::string &vcodec, std::string &ofilepath);
    // performs subtitle overlay operation
    void convertVideo(std::string &extn, std::string &vcodec, std::string &acodec);
    // performs convert operation
    void getGif(std::string &seektime, std::string &duration, std::string &height, std::string &width, std::string &ofilepath, std::string &framerate);
    // performs gif convert operation
    void concat(std::string &ofilepath);
    // concates files
    void muteAudio(std::string &ofilepath);
    // mutes audio channel
    void omitClip(std::string &seektime_ini, std::string &seektime_fn, std::string &ofilepath);
    // omits clip segment
    void setSpeed(std::string &ofilepath, double speed);
    // changes clip speed
    void setRes(std::string &height, std::string &width, std::string &ofilepath);
    // changes clip resolution
    void setVol(std::string &vol, std::string &ofilepath);
    // changes clip volume
    void setBitrate(std::string &bitrate, std::string &ofilepath);
    // changes video bitrate
};
// class definations
fx::fx(std::string &fname)
    : fname(fname) {}

void fx::exec(std::string &arg)
{
    system(("ffplay " + arg).c_str());
}

void fx::extractFrame(std::string &seektime, std::string &height, std::string &width, std::string &ofilepath)
{
    std::string cterm;
    cterm = this->fname + " -ss " + seektime + " -vf scale=" + height + ":" + width + " -vframes 1 " + ofilepath;
    system((TERMINAL + cterm).c_str());
}

void fx::extractAudio(std::string &seektime, std::string &bitrate, std::string &ofilepath)
{
    std::string cterm;
    cterm = this->fname + " -ss " + seektime + " -vn  -ab " + bitrate + " " + ofilepath;
    system((TERMINAL + cterm).c_str());
}

void fx::extractClip(std::string &seektime, std::string &duration, std::string &ofilepath)
{
    std::string cterm;
    cterm = this->fname + " -ss " + seektime + " -codec copy -t " + duration + " " + ofilepath;
    system((TERMINAL + cterm).c_str());
}

void fx::splitVideo(std::string &splitseek, std::string &ofilepath1, std::string &ofilepath2)
{
    std::string cterm;
    cterm = this->fname + " -t " + splitseek + " -c copy " + ofilepath1;
    cterm = cterm + " -ss " + splitseek + " -codec copy " + ofilepath2;
    system((TERMINAL + cterm).c_str());
}
void fx::overlayAudio(std::string &audiofile, std::string &ofilepath)
{
    std::string cterm;
    cterm = this->fname + " -i " + audiofile + " -c:v copy -c:a aac -strict experimental " + ofilepath;
    system((TERMINAL + cterm).c_str());
}
void fx::overlaySub(std::string &subfile, std::string &vcodec, std::string &ofilepath)
{
    std::string cterm;
    cterm = this->fname + " -i " + subfile + " -c copy -c:v " + vcodec + " -crf 23 -preset veryfast " + ofilepath;
    system((TERMINAL + cterm).c_str());
}
void fx::convertVideo(std::string &extn, std::string &vcodec, std::string &acodec)
{
    std::string cterm;
    cterm = this->fname + " -c:v " + vcodec + " -c:a " + acodec + this->fname + extn;
    system((TERMINAL + cterm).c_str());
}

void fx::getGif(std::string &seektime, std::string &duration, std::string &height, std::string &width, std::string &ofilepath, std::string &framerate)
{
    std::string cterm;
    cterm = this->fname + " -ss " + seektime + " -vf scale=" + height + ":" + width;
    cterm = cterm + " -t " + duration + " -r " + framerate + " " + ofilepath;
    system((TERMINAL + cterm).c_str());
}

void fx::concat(std::string &ofilepath)
{
    std::string cterm;
    cterm = "inputlist.txt -c copy " + ofilepath;
    cterm = "ffmpeg -f concat -i " + cterm;
    system(cterm.c_str());
}

std::string fx::getfname()
{
    return fname;
}
void fx::setfname(std::string fname)
{
    this->fname = fname;
}
void fx::muteAudio(std::string &ofilepath)
{
    std::string cterm;
    cterm = this->fname + " -an " + ofilepath;
    system((TERMINAL + cterm).c_str());
}

void fx::omitClip(std::string &seektime_ini, std::string &seektime_fn, std::string &ofilepath)
{
    system(("ffmpeg -i " + this->fname + " -codec copy -t " + seektime_ini + " tempf1.mp4").c_str());
    system(("ffmpeg -i " + this->fname + " -ss " + seektime_ini + " -codec copy tempf2.mp4").c_str());

    std::ofstream fout("tempinputlist.txt", std::ios::ate | std::ios::out);
    fout << "file 'tempf1.mp4'" << std::endl;
    fout << "file 'tempf2.mp4'" << std::endl;
    fout.close();

    system(("ffmpeg -f concat -i tempinputlist.txt -c copy " + ofilepath).c_str());
}

void fx::setSpeed(std::string &ofilepath, double speed)
{
    std::string cterm;
    speed = 1 / speed;
    cterm = this->fname + " -filter:v \"setpts=" + std::to_string(speed) + "*PTS\" " + ofilepath;
    system((TERMINAL + cterm).c_str());
}

void fx::setRes(std::string &height, std::string &width, std::string &ofilepath)
{
    std::string cterm;
    cterm = this->fname + " -vf scale=" + height + ":" + width;
    cterm = cterm + " -c:a copy " + ofilepath;
    system((TERMINAL + cterm).c_str());
}

void fx::setVol(std::string &vol, std::string &ofilepath)
{
    std::string cterm;
    cterm = this->fname + " -af 'volume=" + vol + "' " + ofilepath;
    system((TERMINAL + cterm + ofilepath).c_str());
}

void fx::setBitrate(std::string &bitrate, std::string &ofilepath)
{
    std::string cterm;
    cterm = this->fname + " -b:v " + bitrate + " -bufsize " + bitrate + " ";
    cterm = cterm + ofilepath;
    system((TERMINAL + cterm).c_str());
}