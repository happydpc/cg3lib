/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Alessandro Muntoni (muntoni.alessandro@gmail.com)
 */

#include "drawable_mixed_objects.h"
#include "../opengl_objects/opengl_objects.h"
#include "../opengl_objects/2d/opengl_objects2d.h"


namespace cg3 {

DrawableMixedObjects::DrawableMixedObjects()
{
}

void DrawableMixedObjects::draw() const
{
    for (const Sphere& s : spheres){
        opengl::drawSphere(s.center, s.radius, s.color, s.precision);
    }
    for (const Point& p : points){
        opengl::drawPoint(p.p, p.color, p.size);
    }
    for (const Cylinder& c : cylinders){
        opengl::drawCylinder(c.a, c.b, c.radius, c.radius, c.color);
    }
    for (const Line& l : lines){
        opengl::drawLine(l.a, l.b, l.color, l.width);
    }
    for (const Triangle& t : triangles){
        opengl::drawTriangle(t.a, t.b, t.c, t.color, t.width, t.fill);
    }
}

Pointd DrawableMixedObjects::sceneCenter() const
{
    if (bb.diag() > 0)
        return bb.center();
    return Pointd();
}

double DrawableMixedObjects::sceneRadius() const
{
    //if (bb.diag() > 0)
    //    return bb.diag()/2;
    return -1;
}

void DrawableMixedObjects::updateBoundingBox()
{
    if (numberObjects() == 0) {
        bb = BoundingBox();
    }
    else {
        const double dmax = std::numeric_limits<double>::max();
        const double dmin = std::numeric_limits<double>::min();
        bb = BoundingBox(Pointd(dmax, dmax, dmax), Pointd(dmin, dmin, dmin));
    }
    for (const Sphere& s : spheres){
        bb.min() = bb.min().min(s.center);
        bb.max() = bb.max().max(s.center);
    }
    for (const Point& p : points){
        bb.min() = bb.min().min(p.p);
        bb.max() = bb.max().max(p.p);
    }
    for (const Cylinder& c : cylinders){
        bb.min() = bb.min().min(c.a);
        bb.min() = bb.min().min(c.b);
        bb.max() = bb.max().max(c.a);
        bb.max() = bb.max().max(c.b);
    }
    for (const Line& l : lines){
        bb.min() = bb.min().min(l.a);
        bb.min() = bb.min().min(l.b);
        bb.max() = bb.max().max(l.a);
        bb.max() = bb.max().max(l.b);
    }
}

unsigned int DrawableMixedObjects::numberObjects() const
{
    return (unsigned int) (spheres.size() + cylinders.size() + lines.size());
}

unsigned int DrawableMixedObjects::addSphere(const Pointd& center, double radius, const QColor& color, int precision)
{
    Sphere s = {center, radius, color, precision};
    spheres.push_back(s);
    bb.min() = bb.min().min(center);
    bb.max() = bb.max().max(center);
    return spheres.size()-1;
}

void DrawableMixedObjects::clearSpheres()
{
    spheres.clear();
    updateBoundingBox();
}

unsigned int DrawableMixedObjects::addPoint(const Pointd& p, const QColor& color, int size)
{
    Point pp = {p, color, size};
    points.push_back(pp);
    bb.min() = bb.min().min(p);
    bb.max() = bb.max().max(p);
    return points.size()-1;
}

void DrawableMixedObjects::clearPoints() {
    points.clear();
    updateBoundingBox();
}

unsigned int DrawableMixedObjects::addCylinder(const Pointd& a, const Pointd& b, double radius, const QColor color)
{
    Cylinder c = {a, b, radius, color};
    cylinders.push_back(c);

    bb.min() = bb.min().min(a);
    bb.min() = bb.min().min(b);
    bb.max() = bb.max().max(a);
    bb.max() = bb.max().max(b);

    return cylinders.size()-1;
}

void DrawableMixedObjects::clearCylinders()
{
    cylinders.clear();
    updateBoundingBox();
}

unsigned int DrawableMixedObjects::addLine(const Pointd &a, const Pointd &b, const QColor color, int width)
{
    Line l = {a, b, width, color};
    lines.push_back(l);
    bb.min() = bb.min().min(a);
    bb.min() = bb.min().min(b);
    bb.max() = bb.max().max(a);
    bb.max() = bb.max().max(b);
    return lines.size()-1;
}

void DrawableMixedObjects::clearLines()
{
    lines.clear();
    updateBoundingBox();
}

unsigned int DrawableMixedObjects::addTriangle(const Pointd& a, const Pointd& b, const Pointd& c, const QColor color, int width, bool fill)
{
    Triangle t {a, b, c, width, color, fill};
    triangles.push_back(t);
    bb.min() = bb.min().min(a);
    bb.min() = bb.min().min(b);
    bb.min() = bb.min().min(c);
    bb.max() = bb.max().max(a);
    bb.max() = bb.max().max(b);
    bb.max() = bb.max().max(c);

    return triangles.size()-1;
}

void DrawableMixedObjects::clearTriangles()
{
    triangles.clear();
    updateBoundingBox();
}

} //namespace cg3