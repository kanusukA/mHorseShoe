#pragma once

#ifndef GLOCK_H
#define GLOCK_H

#include<chrono>
#include<stdio.h>
#include<iostream>

double getCurrentTime();

int getFPS(int framesPerMilli);

#define MS_PRE_FRAME 16.0f


class Glock {
	auto startTime;
	auto endTime;
public:

	double deltaTime = 0;

	void setStartTime() {
		startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
	}

	void setEndTime() {
		endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
	}

	double getDelta() {
		return std::chrono::duration<double>(endTime - startTime);
	}

	void setSleep(float duration) {
		Sleep(duration);
	}



};


#endif // !GLOCK_H

