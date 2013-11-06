//
//  CalibrationData.cpp
//  mapamokWWMMM
//
//  Created by Saqoosha on 2013/11/06.
//
//

#include "CalibrationData.h"


ofImage CalibrationMesh::random;
float CalibrationMesh::white = 0.0f;
float CalibrationMesh::visibility = 1.0f;


void CalibrationMesh::setup() {
    random.loadImage("random.png");
    random.getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
}


CalibrationMesh::CalibrationMesh(ofMesh &mesh) : useTexture(false), useShader(false), polygonOffset(0) {
    object_mesh = mesh;
    int n = object_mesh.getNumVertices();
    points.resize(n);
    reset();
}


void CalibrationMesh::reset() {
    for (int i = 0; i < points.size(); i++) {
        points[i].object = object_mesh.getVertex(i);
        points[i].image.set(0);
        points[i].enabled = false;
    }
}


void CalibrationMesh::project() {
    projected_mesh = getProjectedMesh(object_mesh);
}


ofVec3f CalibrationMesh::getProjected(int index) {
    return projected_mesh.getVertex(index);
}


bool CalibrationMesh::loadTexture(string filename) {
    ofPixels pixs;
    useTexture = ofLoadImage(pixs, filename);
    if (useTexture) {
        texture.setCompression(OF_COMPRESS_ARB);
        texture.loadData(pixs);
    }
    return useTexture;
}


bool CalibrationMesh::loadShader(string filename) {
    shader_name = filename;
    useShader = shader.load(filename);
    return useShader;
}


void CalibrationMesh::reloadShader() {
    shader.unload();
    useShader = shader.load(shader_name);
}


void CalibrationMesh::customDraw() {
    ofPushStyle();
    if (polygonOffset) {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, polygonOffset);
    }
    if (useShader) {
        shader.begin();
        shader.setUniformMatrix4f("modelMatrix", getLocalTransformMatrix());
        shader.setUniform1f("tick", ofGetElapsedTimef());
        shader.setUniformTexture("random", CalibrationMesh::random, 0);
        if (useTexture) shader.setUniformTexture("tex", texture, 1);
        shader.setUniform1f("white", CalibrationMesh::white);
        shader.setUniform1f("visibility", CalibrationMesh::visibility);
    }
    object_mesh.drawFaces();
    if (useShader) shader.end();
    if (polygonOffset) glDisable(GL_POLYGON_OFFSET_FILL);
    ofPopStyle();
}

