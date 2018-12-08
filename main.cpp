#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include <iostream>
#include <cmath>

const double OPACITYINCREMENT = 0.05;
const float RADIANINCREMENT = 0.05;
const int TICKRATE = 1;
const int BACKGROUND [3] = {0, 15, 0};
const int RADARLINECOLOR[3] = {1, 255, 10};
const QColor RADARPLATECOLOR(40, 40, 50);

int randNum(int min, int max)  //Generate a random number.
{
    unsigned int N = (max - min <= RAND_MAX)
        ? (max - min + 1U)
        : (RAND_MAX + 1U);
    unsigned int x = (RAND_MAX + 1U) / N;
    unsigned int y = x * N;
    unsigned int r;
    do {
        r = rand();
    } while (r >= y);
    return r / x + min;
}

//Youve implemeted the algorithim to get the end point, just need to draw the line from start to finish
QImage radarLine(QImage image, int width, int height, float count, int red, int green, int blue, float opacity = 0.8){
    int center[2] = {width / 2, height / 2};
    int yCount = 0;
    int radius = (width + height) / 4;
    QColor color(red * opacity, green * opacity + BACKGROUND[1], blue * opacity);
    int x0 = center[0], y0 = center[1];
    //Gets the point to draw to.
    int x1 = (radius * cos(count)) + center[0], y1 = (radius * sin(count)) + center[1];
    image.setPixelColor(x1, y1, color);
    //If the center is bigger than the target coordinate, swap which way to draw from.
    if(x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float deltax = x1 - x0;
    float deltay = y1 - y0;
    double deltaError;
    //Get the angle to draw the line at.
    if(deltax != 0 ){
        deltaError = fabs(deltay / deltax);
    }else{
        deltaError = 0;
    }
    float error = 0.0;
    int y = y0;
    //Change color of the pixel, according to deltaError, incrementing the y coordinate of the pixel if error is within bounds to do so.
    for(int x = x0; x <= x1; x++){
        image.setPixelColor(x, y, color);
        error += fabs(deltaError);

        while(error >= 0.5 && x <= x1){

            y += ((deltay > 0) - (deltay < 0));
            image.setPixelColor(x, y, color);
            error -= 1;
            yCount++;
            //if pixel has travelled to the center of the image, prevent it from drawing anymore.
            if(yCount >= center[1]){
                x = x1 + 1;
            }
        }
    }


    return image;
}

//Draws a circle in the center of a provided image, using the provided image, dimensions, and colors..
QImage drawCircle(QImage image, int width, int height, int red, int green, int blue){
    int center[2] = {width / 2, height / 2};
    int radius = (width + height) / 4;
    int x = center[0], y = center[1] + radius;
    QColor color(red, green, blue);
    //Iterate through, drawing a circle based on distance from the center point.
    for(y = -radius; y <= radius; y++){
        for(x = -radius; x<= radius; x++){
            if(x*x+y*y <= radius*radius){
                image.setPixelColor(x + center[0], y + center[1], color);
            }
        }
    }
    return image;
}

//Asynch wait function.
void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(TICKRATE);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));//Seed the random function
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    bool stop = false;
    int dimensionX = 300, dimensionY = 300, backgroundRed = BACKGROUND[0], backgroundGreen = BACKGROUND[1], backgroundBlue = BACKGROUND[2];
    double count = 0.0, i;
    QImage radarPlate;
    QColor backgroundColor(BACKGROUND[0], BACKGROUND[1], BACKGROUND[2]);
    QLabel *label = new QLabel(
        QApplication::translate("childwidget", ""), &w);
    radarPlate = QImage(dimensionX, dimensionY, QImage::Format_RGB32);
    //set the background color.
    radarPlate.fill(RADARPLATECOLOR);
    radarPlate = drawCircle(radarPlate, dimensionX, dimensionY, backgroundRed, backgroundGreen, backgroundBlue);
    //Resize and set the image.
    label->setPixmap(QPixmap::fromImage(radarPlate));
    label->resize(dimensionX, dimensionY);
    label->move(100, 100);
    int green [3] = {RADARLINECOLOR[0], RADARLINECOLOR[1], RADARLINECOLOR[2]};
    //Draw line, iterate through and draw darker lines behind the main line to provide a gradient.
    do{
        //radarLine accepts an image, a x coordinate, a y coordinate, a float(radians) and three ints corresponding to red, green, blue, color of the line, and returns an image with the line drawn.
        for(i = -20; i <= 0; i++){
            radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count + i * RADIANINCREMENT, green[0], green[1], green[2], 1 - fabs(i * OPACITYINCREMENT));
        }
        count += 0.005;
        label->setPixmap(QPixmap::fromImage(radarPlate));
        label->show();
        delay();
    }while(stop != true);

    return a.exec();
}
