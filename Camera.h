#ifndef CAMERA_H
#define CAMERA_H

class Camera {
private:
	int width, height;
    int x, y;
    float zoomFactor;

public:
    Camera(int w, int h) : width(w), height(h), x(0), y(0), zoomFactor(1.0f) {}

    int getX() const { return x; }
    int getY() const { return y; }
    float getZoomFactor() const { return zoomFactor; }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    void zoomIn() {
        zoomFactor *= 1.1f;  // Zoom in by 10%
    }

    void zoomOut() {
        zoomFactor /= 1.1f;  // Zoom out by 10%
    }

    void applyZoom(int &adjustedX, int &adjustedY) const {
        adjustedX = int( (adjustedX-width/2) * zoomFactor) + width/2;
        adjustedY = int( (adjustedY-height/2) * zoomFactor) + height/2;
    }
};


#endif
