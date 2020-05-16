#pragma once

#include <iostream>
#include <thread>
#include <ctime>

class Utility
{
	inline static void sleepFunction(double delaySeconds)
	{
        std::clock_t tick = std::clock();
        do {} while ((std::clock() - tick) / (double)CLOCKS_PER_SEC < delaySeconds);
	}

public:

	inline static void report(const std::string& type, const std::string& text)
	{
		std::cout << type << "=[" << text << "]\n";
	}

	inline static void pressKey()
	{
		std::cout << "Press any key to coninue...\n";
		int x;
		std::cin >> x;
	}

	inline static void sleep(double delaySeconds)
	{
		std::thread sleepThread;
		sleepThread = std::thread
		{
			sleepFunction, delaySeconds
		};
		sleepThread.join();
	}

    inline static bool gameObjectsOverlap(GameObject* pGo1, GameObject* pGo2)
    {
        double r1 = pGo1->computeRadius();
        double r2 = pGo2->computeRadius();
        Vector2D p1 = pGo1->readPosition();
        Vector2D p2 = pGo2->readPosition();
        return (p1 - p2).getMagnitude() < r1 + r2;
    }

    inline static void drawCircle(SDL_Renderer* renderer, int x0, int y0, unsigned int r)
    {
        const int diameter = (r * 2);

        int x = (r - 1);
        int y = 0;
        int tx = 1;
        int ty = 1;
        int error = (tx - diameter);

        while (x >= y)
        {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
            SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
            SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
            SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
            SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
            SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
            SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
            SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0)
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }
};
