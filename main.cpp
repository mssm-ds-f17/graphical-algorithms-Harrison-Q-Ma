#include "graphics.h"
#include "networkplugin.h"
#include <iostream>
#include <QThread>
#include <limits>

#include "filedownloader.h"

using namespace std;
using namespace mssm;

class Segment
{
public:
    double distance;
    Vec2d point1;
    Vec2d point2;
public:
    Segment();
    Segment(Vec2d p1,Vec2d p2);
    ~Segment();
};

void printVec2d(Vec2d pt)
{
    cout << "(" << pt.x << "," << pt.y << ")" << endl;
}

void printSegment(Segment s)
{
    cout << "The distance between points is: " << s.distance << endl;
    cout << "The first point is: ";
    printVec2d(s.point1);
    cout << "The second point is: ";
    printVec2d(s.point2);
}

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

void drawPt(Graphics& g, Vec2d center, Color c)
{
    g.ellipse(center,5,5,c,c);
}

bool compareByX(Vec2d v1, Vec2d v2)
{
    if(v1.x < v2.x)
    {
        return true;
    }
    return false;
}

bool compareByY(Vec2d v1, Vec2d v2)
{
    if(v1.y < v2.y)
    {
        return true;
    }
    return false;
}

double dist(Vec2d p1, Vec2d p2)
{
    double dist;
    double diffX = p1.x - p2.x;
    double diffY = p1.y - p2.y;

    dist = sqrt(pow(diffX,2)+pow(diffY,2));

    return dist;
}

Segment::Segment()
{

}

Segment::Segment(Vec2d p1, Vec2d p2)
{
    point1 = p1;
    point2 = p2;
    distance = dist(p1,p2);
}

Segment::~Segment()
{

}

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

    return pts;
}

double midLine(Graphics& g,vector<Vec2d> vec)
{
    cout << vec.size() << endl;

    double midPt = 0;

    if(vec.size()%2 != 0)
    {
        midPt = vec[floor(vec.size()/2)].x;
        g.line({midPt,0},{midPt,(double)g.height()});
    }
    else
    {
        midPt = (vec[vec.size()/2-1].x + vec[vec.size()/2].x)/2;
        g.line({midPt,0},{midPt,(double)g.height()});
    }

    return midPt;
}

Segment bruteForce(Graphics& g,vector<Vec2d>& vec)
{
    Segment s;
    s.distance = numeric_limits<double>::max();

    for(unsigned int i = 0; i < vec.size() - 1; i++)
    {
        for(unsigned int j = i+1; j < vec.size(); j++)
        {
            double xDist = vec[i].x - vec[j].x;
            double yDist = vec[i].y - vec[j].y;
            double distance = sqrt(pow(xDist,2)+pow(yDist,2));

            if(distance < s.distance)
            {
                s.distance = distance;
                s.point1 = vec[i];
                s.point2 = vec[j];
            }
        }
    }

    drawPt(g,s.point1,GREEN);
    drawPt(g,s.point2,GREEN);

    return s;
}

Segment bruteForce(vector<Vec2d>& vec)
{
    Segment s;
    s.distance = numeric_limits<double>::max();

    for(unsigned int i = 0; i < vec.size() - 1; i++)
    {
        for(unsigned int j = i+1; j < vec.size(); j++)
        {
            double xDist = vec[i].x - vec[j].x;
            double yDist = vec[i].y - vec[j].y;
            double distance = sqrt(pow(xDist,2)+pow(yDist,2));

            if(distance < s.distance)
            {
                s.distance = distance;
                s.point1 = vec[i];
                s.point2 = vec[j];
            }
        }
    }

    return s;
}

Segment closestPointsRecursive(Graphics& g, vector<Vec2d> points)
{
    if(points.size() <= 3)
    {
        return bruteForce(points);
    }

    Segment s;

    unsigned int midPoint = floor(points.size()/2);

    vector<Vec2d> subVector1;
    vector<Vec2d> subVector2;

    subVector1.resize(midPoint);
    subVector2.resize(points.size()-midPoint);

    double dist1 = 0;
    double dist2 = 0;
    double closestDist = 0;

    for(unsigned int i = 0; i < midPoint; i++)
    {
        subVector1[i] = points[i];
    }
    for(unsigned int i = midPoint; i < points.size();i++)
    {
        subVector2[i-midPoint] = points[i];
    }

    closestPointsRecursive(g,subVector1);
    closestPointsRecursive(g,subVector2);


    if(dist1 < closestDist || dist2 < closestDist)
    {
        if(dist1 < dist2)
        {
            closestDist = dist1;
        }
        else
        {
            closestDist = dist2;
        }
    }

    vector<Vec2d> strip;

    for(unsigned int i = 0; i < points.size(); i++)
    {
        if(points[i].x >= (midPoint - closestDist) || points[i].x <= (midPoint + closestDist))
        {
            strip.push_back(points[i]);
        }
    }

    sort(strip.begin(),strip.end(),compareByY);

    double minDist = numeric_limits<double>::max();
    double tempDist = 0;

    for(unsigned int i = 0; i < strip.size()-1; i++)
    {
        for(unsigned int j = i+1; j < strip.size(); j++)
        {
            tempDist = dist(strip[i],strip[j]);
            if((strip[i].y - strip[j].y) <= closestDist)
            {
                if(tempDist < minDist)
                {
                    minDist = tempDist;
                    s.distance = minDist;
                    s.point1 = strip[i];
                    s.point2 = strip[j];
                }
            }
        }
    }

    return s;
}

Segment closestPoints(Graphics& g,vector<Vec2d>& points)
{
    // first sort by x
    sort(points.begin(),points.end(),compareByX);

    // call the recursive part
    return closestPointsRecursive(g,points);
}

void graphicsMain(Graphics& g)
{
    vector<Vec2d> vec = drawRandomPts(g,{0,0},{(double)g.width(),(double)g.height()},10);

    cout << "Result from Brute Force:" << endl;

    bruteForce(g,vec);
    printSegment(bruteForce(g,vec));

    cout << "Result from the smart way to do it" << endl;

    closestPoints(g,vec);
    printSegment(closestPoints(g,vec));

    while (g.draw())
    {
        drawPt(g,bruteForce(g,vec).point1,GREEN);
        drawPt(g,bruteForce(g,vec).point2,GREEN);

        drawPt(g,closestPoints(g,vec).point1,RED);
        drawPt(g,closestPoints(g,vec).point2,RED);
    }
}

int main()
{
    cout << "Main Thread" << QThread::currentThreadId() << endl;

    Graphics g("Closest Points", 900, 900, graphicsMain);
}
