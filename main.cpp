#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include <iostream>
#include <cmath>

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
QImage radarLine(QImage image, int width, int height, int count){
    int center[2] = {width / 2, height / 2};
    double radian = (count * M_PI);
    int radius = (width + height) / 4;
    bool x;
    int x0 = center[0], y0 = center[1], red = 255, green = 5, blue = 5;
    QColor color = (Qt::green);
    QColor color2 = (Qt::red);
    int x1 = (radius * cos(count)) + 150, y1 = (radius * sin(count)) + 150;
    image.setPixelColor(x1, y1, color2);
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
        image.setPixelColor(x, y, color2);
        error += fabs(deltaError);

        while(error >= 0.5){

            y += ((deltay > 0) - (deltay < 0));
            image.setPixelColor(x, y, color2);
            error -= 1;
        }
    }


    return image;
}

QImage drawCircle(QImage image, int width, int height){
    int center[2] = {width / 2, height / 2};
    int radius = (width + height) / 4;
    int x = center[0], y = center[1] + radius, red = 255, green = 255, blue = 255;
    QColor color = (red, green, blue);
    for(y = -radius; y <= radius; y++){
        for(x = -radius; x<= radius; x++){
            if(x*x+y*y <= radius*radius){
                image.setPixelColor(x + center[0], y + center[1], color);
            }
        }
    }
    return image;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));//Seed the random function
    QApplication a(argc, argv);
    QImage pi;
    QPainter p(&pi);
    MainWindow w;
    w.show();
    int dimensionX = 300, dimensionY = 300, count = -780;
    QColor color = (30, 30, 30);
    QLabel *label = new QLabel(
        QApplication::translate("childwidget", ""), &w);
    QImage radarPlate;
    radarPlate = QImage(dimensionX, dimensionY, QImage::Format_RGB32);
    radarPlate.fill(color);
    radarPlate = drawCircle(radarPlate, dimensionX, dimensionY);
    label->setPixmap(QPixmap::fromImage(radarPlate));
    do{
        radarPlate = radarLine(radarPlate, dimensionX, dimensionY, count);
        count++;
        label->setPixmap(QPixmap::fromImage(radarPlate));
    }while(count < 780);
    label->resize(dimensionX, dimensionY);
    label->move(100, 100);
    label->show();

    return a.exec();
}
