//
// Created by rdelfin on 11/26/16.
//

#pragma once

class Renderable {
public:
    Renderable();


    virtual void draw() = 0;

    virtual ~Renderable();
protected:
    Shader vertexShader, geometryShader, fragmentShader;
};
