#include "graphics.h"
#include "networkplugin.h"
#include <iostream>
#include <QThread>
#include <cfloat>

#include "filedownloader.h"

using namespace std;
using namespace mssm;

// working
void printVec(vector<Vec2d> vec)
{
    for(unsigned int i = 0; i < vec.size(); i++)
    {
        if(i == 0)
        {
            cout << "<" << vec[i].x << " , " << vec[i].y << endl;
        }
        else if(i == vec.size()-1)
        {
            cout << " " << vec[i].x << " , " << vec[i].y << ">" << endl;
        }
        else
        {
            cout << " " << vec[i].x << " , " << vec[i].y << endl;
        }
    }
}

// working
void printVec2d(Vec2d pt)
{
    cout << "(" << pt.x << "," << pt.y << ")" << endl;
}

// working
void drawPt(Graphics& g, Vec2d center, Color c)
{
    g.line(center,center,c);
}

// working
bool compareByX(Vec2d v1, Vec2d v2)
{
    if(v1.x < v2.x)
    {
        return true;
    }
    return false;
}

// should work
bool compareByY(Vec2d v1, Vec2d v2)
{
    if(v1.y < v2.y)
    {
        return true;
    }
    return false;
}

// working
double distance(Vec2d p1, Vec2d p2)
{
    double dist;
    double diffX = p1.x - p2.x;
    double diffY = p1.y - p2.y;

    dist = sqrt(pow(diffX,2)+pow(diffY,2));

    return dist;
}

// working
vector<Vec2d> drawRandomPts(Graphics& g, Vec2d upperLeft, Vec2d lowerRight, int num)
{
    vector<Vec2d> pts;

    for(int i = 0; i < num; i++)
    {
        double randomX = g.randomDouble(upperLeft.x,lowerRight.x);
        double randomY = g.randomDouble(upperLeft.y,lowerRight.y);

        drawPt(g,{randomX,randomY},WHITE);

        pts.push_back({randomX,randomY});
    }

    sort(pts.begin(),pts.end(),compareByX);

    return pts;
}

// working
void midLine(Graphics& g,vector<Vec2d> vec)
{
    cout << vec.size() << endl;

    if(vec.size()%2 != 0)
    {
        double midPt = vec[floor(vec.size()/2)].x;
        g.line({midPt,0},{midPt,(double)g.height()});
    }
    else
    {
        double midPt = (vec[vec.size()/2-1].x + vec[vec.size()/2].x)/2;
        g.line({midPt,0},{midPt,(double)g.height()});
    }
}

// working
double bruteForce(vector<Vec2d> vec)
{
    double closestDist = DBL_MAX;
    Vec2d pt1;
    Vec2d pt2;

    for(unsigned int i = 0; i < vec.size() - 1; i++)
    {
        double xDist = vec[i].x - vec[i+1].x;
        double yDist = vec[i].y - vec[i+1].y;
        double distance = sqrt(pow(xDist,2)+pow(yDist,2));

        if(distance < closestDist)
        {
            closestDist = distance;
            pt1 = vec[i];
            pt2 = vec[i+1];
        }
    }

    printVec2d(pt1);
    printVec2d(pt2);

    return closestDist;
}

/*
double minDist(vector<Vec2d> vec, unsigned int startIndex, unsigned int endIndex)
{
    double minDistance;
}
*/

void graphicsMain(Graphics& g)
{
    vector<Vec2d> vec = drawRandomPts(g,{0,0},{(double)g.width(),(double)g.height()},10);

    printVec(vec);

    midLine(g,vec);

    bruteForce(vec);

    while (g.draw())
    {

    }
}

int main()
{
    cout << "Main Thread" << QThread::currentThreadId() << endl;

    Graphics g("Closest Points", 900, 900, graphicsMain);
}
