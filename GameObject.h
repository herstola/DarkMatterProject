#pragma once

#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include "Vector2D.h"
#include "Angle.h"

class Texture
{
	SDL_Texture* pSdlTexture;
	int width;
	int height;

public:
	Texture()
	{
		pSdlTexture = 0;
		width = 0;
		height = 0;
	}

	~Texture()
	{
		//Deallocate
		free();
	}

	bool loadFromFile(SDL_Renderer* renderer, std::string path)
	{
		free();
		SDL_Texture* pTexture = 0;
		SDL_Surface* pSurface = IMG_Load(path.c_str());

		if (pSurface == 0)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}
		else
		{
			SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0, 0xFF, 0xFF));
			pTexture = SDL_CreateTextureFromSurface(renderer, pSurface);
			if (pTexture == 0)
			{
				printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			}
			else
			{
				width = pSurface->w;
				height = pSurface->h;
			}

			SDL_FreeSurface(pSurface);
		}

		pSdlTexture = pTexture;
		return pSdlTexture != 0;
	}

	void free()
	{
		if (pSdlTexture != 0)
		{
			SDL_DestroyTexture(pSdlTexture);
			pSdlTexture = 0;
			width = 0;
			height = 0;
		}
	}

	void setColor(Uint8 red, Uint8 green, Uint8 blue)
	{
		SDL_SetTextureColorMod(pSdlTexture, red, green, blue);
	}

	void setBlendMode(SDL_BlendMode blending)
	{
		SDL_SetTextureBlendMode(pSdlTexture, blending);
	}

	void setAlpha(Uint8 alpha)
	{
		SDL_SetTextureAlphaMod(pSdlTexture, alpha);
	}

	// Renders texture at given point
	void render(SDL_Renderer* renderer, Vector2D position, 
		SDL_Rect* clip = 0, double angle = 0.0, 
		SDL_Point* center = 0, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		SDL_Rect renderQuad = 
		{ 
			static_cast<int>(position[0]), 
			static_cast<int>(position[1]),
			width, 
			height 
		};

		if (clip != 0)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		// Render to screen
		SDL_RenderCopyEx(renderer, pSdlTexture, clip, 
			&renderQuad, angle, center, flip);
	}

	// Gets image dimensions
	int getWidth() const
	{
		return width;
	}

	int getHeight() const
	{
		return height;
	}
};

class GameObject
{
	Texture texture;
	AngleDeg heading;
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;
	double speed;
	double age;

public:
	GameObject() :
		texture(),
		heading(AngleDeg(0.0)),
		position(Vector2D(0.0, 0.0)),
		speed(0.0),
		age(0.0)
	{}

	~GameObject()
	{}

	Texture& accessTexture() { return texture; }
	const double& readAge() const { return age; }
	const AngleDeg& readHeading() const { return heading; }
	const Vector2D& readPosition() const { return position; }
	void overrideAge(double age) { age = age; }
	void overrideHeading(const AngleDeg& h) { heading = h; }
	void overridePosition(const Vector2D& p) { position = p; }
	
	double computeRadius() const {
		int height = texture.getHeight();
		int width = texture.getWidth();
		int sumOfProd = height * height + width * width;
		double sqrtSumOfProd = sqrt(sumOfProd);
		int length = height > width ? width : height;
		return 0.25 * (length + sqrtSumOfProd);
	}

	Vector2D getVelocity() const
	{
		// Compute velocity
		double uvx = sin(heading.getRad() - PI);
		double uvy = cos(heading.getRad());
		Vector2D uv = Vector2D(uvx, uvy);
		return uv * speed;
	}

	void setVelocity(const Vector2D& v)
	{
		//speed = 
	}

	void virtual propagate(double dt)
	{
		// Update position
		position += getVelocity() * dt;
	}

	void render(SDL_Renderer* renderer)
	{
		texture.render(renderer, position);
	}
};

