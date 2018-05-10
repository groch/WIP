#include <iostream>
#include <fstream>
#include <math.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <stdlib.h>

#include "IL\ilut.h"

#include "render.h"

// credit to https://r3dux.org/tag/ilutglloadimage/
unsigned int loadImage(const char* theFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint
	unsigned int textureID;			// Create a texture ID as a GLuint
	ILboolean success;			// Create a flag to keep track of success/failure
	ILenum error;				// Create a flag to keep track of the IL error state
	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(theFileName); 	// Load the image file

	// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		// not usefull as assimp already flip UV
//		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
//		{
//			iluFlipImage();
//		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(ImageInfo.Format, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
			exit(-1);
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
					 0,				// Pyramid level (for mip-mapping) - 0 is the top level
					 ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
					 ilGetInteger(IL_IMAGE_WIDTH),	// Image width
					 ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
					 0,				// Border width in pixels (can either be 1 or 0)
					 ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
					 GL_UNSIGNED_BYTE,		// Image data type
					 ilGetData());			// The actual image data itself
        glGenerateMipmap(GL_TEXTURE_2D);
 	}
  	else // If we failed to open the image file in the first place...
  	{
		error = ilGetError();
		std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
		exit(-1);
  	}

 	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
	std::cout << "Texture creation successful.ID=" << textureID << std::endl;
	return textureID; // Return the GLuint to the texture so you can use it!
}
