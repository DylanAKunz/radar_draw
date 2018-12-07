#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include <iostream>
#include <cmath>

const int BACKGROUND [3] = {0, 15, 0};
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
    int x1 = (radius * cos(count)) + 150, y1 = (radius * sin(count)) + 150;
    image.setPixelColor(x1, y1, color);
    if(x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float deltax = x1 - x0;
    float deltay = y1 - y0;
    double deltaError;
    if(deltax != 0 ){
        deltaError = fabs(deltay / deltax);
    }else{
        deltaError = 0;
    }
    float error = 0.0;
    int y = y0;
    for(int x = x0; x <= x1; x++){
        image.setPixelColor(x, y, color);
        error += fabs(deltaError);

        while(error >= 0.5 && x <= x1){

            y += ((deltay > 0) - (deltay < 0));
            image.setPixelColor(x, y, color);
            error -= 1;
            yCount++;
            if(yCount >= 150){
                x = x1 + 1;
            }
        }
    }


    return image;
}

QImage drawCircle(QImage image, int width, int height){
    int center[2] = {width / 2, height / 2};
    int radius = (width + height) / 4;
    int x = center[0], y = center[1] + radius;
    QColor color(BACKGROUND[0], BACKGROUND[1], BACKGROUND[2]);
    for(y = -radius; y <= radius; y++){
        for(x = -radius; x<= radius; x++){
            if(x*x+y*y <= radius*radius){
                image.setPixelColor(x + center[0], y + center[1], color);
            }
        }
    }
    return image;
}

void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}

int main(int argc, char *argv[])
{
    bool stop = false;
    srand(time(NULL));//Seed the random function
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int dimensionX = 300, dimensionY = 300;
    float count = 0.0;
    QColor backgroundColor(BACKGROUND[0], BACKGROUND[1], BACKGROUND[2]);
    QLabel *label = new QLabel(
        QApplication::translate("childwidget", ""), &w);
    QImage radarPlate;
    radarPlate = QImage(dimensionX, dimensionY, QImage::Format_RGB32);
    radarPlate.fill(RADARPLATECOLOR);
    radarPlate = drawCircle(radarPlate, dimensionX, dimensionY);
    label->setPixmap(QPixmap::fromImage(radarPlate));
    label->resize(dimensionX, dimensionY);
    label->move(100, 100);
    int green [3] = {1, 255, 10};
    QColor color2 = (Qt::red);
    do{
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count + 0.03, green[0], green[1], green[2], 1.0);
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count - 0.030, green[0], green[1], green[2], 0.8);
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count - 0.045, green[0], green[1], green[2], 0.7);
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count - 0.06, green[0], green[1], green[2], 0.6);
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count - 0.075, green[0], green[1], green[2], 0.5);
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count - 0.09, green[0], green[1], green[2], 0.4);
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count - 0.0105, green[0], green[1], green[2], 0.3);
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count - 0.120, green[0], green[1], green[2], 0.2);
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count - 0.135, green[0], green[1], green[2], 0.1);
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count - 0.150, green[0], green[1], green[2], 0);
        count += 0.005;
        label->setPixmap(QPixmap::fromImage(radarPlate));
        label->show();
        delay();
    }while(stop != true);

    return a.exec();
}
