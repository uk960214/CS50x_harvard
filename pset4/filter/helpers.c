#include "helpers.h"
#include "math.h"
#include "stdio.h"

int cap(int color);
int color(int height, int width, RGBTRIPLE image[height][width], int xIndex, int yIndex, int color);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float red;
    float green;
    float blue;
    float avg;

    // Loop through all pixels
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Get average value of red, green blue of pixel
            red = image[h][w].rgbtRed;
            green = image[h][w].rgbtGreen;
            blue = image[h][w].rgbtBlue;
            avg = (red + green + blue) / 3;

            // Assing average value to pixel
            image[h][w].rgbtRed = round(avg);
            image[h][w].rgbtGreen = round(avg);
            image[h][w].rgbtBlue = round(avg);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy image temporarily
    RGBTRIPLE tempImg[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            tempImg[h][w] = image[h][w];
        }

    }

    // Replace each pixel with the pixel on the opposite side
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = tempImg[h][width - w - 1];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy image for calculation
    RGBTRIPLE tempImg[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            tempImg[h][w] = image[h][w];
        }
    }

    float avgRed;
    float avgGreen;
    float avgBlue;
    int count;

    // Loop through each pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Reset all calculation variables for each pixel
            avgRed = 0;
            avgGreen = 0;
            avgBlue = 0;
            count = 0;

            // Loop through surrounding pixels
            for (int i = h - 1; i < h + 2; i++)
            {
                for (int j = w - 1; j < w + 2; j++)
                {
                    // Do not add into calculation if surrounding pixel is outside of image
                    if (i >= 0 && i < height && j >= 0 && j < width)
                    {
                        avgRed += tempImg[i][j].rgbtRed;
                        avgGreen += tempImg[i][j].rgbtGreen;
                        avgBlue += tempImg[i][j].rgbtBlue;
                        count++;
                    }
                }
            }

            // Get rounded average
            avgRed = round(avgRed / count);
            avgGreen = round(avgGreen / count);
            avgBlue = round(avgBlue / count);

            // Assign average value to image
            image[h][w].rgbtRed = cap(avgRed);
            image[h][w].rgbtGreen = cap(avgGreen);
            image[h][w].rgbtBlue = cap(avgBlue);
        }
    }
    return;
}

// Cap int values with 255
int cap(int color)
{
    // If value is over 255, set to 255
    if (color > 255)
    {
        return color = 255;
    }
    return color;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy image for calculation
    RGBTRIPLE tempImg[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            tempImg[h][w] = image[h][w];
        }
    }

    // Declare array of weight indicies to be multiplied to surrounding pixels
    int weight[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int xIndex;
    int yIndex;

    // Declare array of colors for each pixels' Gx and Gy and final result for the pixel
    float Gx[3];
    float Gy[3];
    float result[3];


    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Reset array for caculation
            for (int r = 0; r < 3; r++)
            {
                Gx[r] = 0;
                Gy[r] = 0;
                result[r] = 0;
            }

            // Loop through colors
            for (int c = 0; c < 3; c++)
            {
                // Reset index for multiplying for each color
                xIndex = 0;
                yIndex = 0;

                // Loop through surrounding pixels in vertical order for Gx
                for (int j = w - 1; j < w + 2; j++)
                {
                    for (int i = h - 1; i < h + 2; i++)
                    {
                        // Only caculate for values in the range of the image
                        if (i >= 0 && i < height && j >= 0 && j < width)
                        {
                            Gx[c] += weight[xIndex] * color(height, width, tempImg, i, j, c);
                        }
                        xIndex++;
                    }
                }

                // Loop through surrounding pixels in horizontal order for Gy
                for (int i = h - 1; i < h + 2; i++)
                {
                    for (int j = w - 1; j < w + 2; j++)
                    {
                        if (i >= 0 && i < height && j >= 0 && j < width)
                        {
                            Gy[c] += weight[yIndex] * color(height, width, tempImg, i, j, c);
                        }
                        yIndex++;
                    }
                }

                // Calculate final value
                float sum = pow(Gx[c], 2) + pow(Gy[c], 2);
                float temp = round(sqrt(sum));
                result[c] = cap(temp);
            }

            // Assign final value to image
            image[h][w].rgbtRed = result[0];
            image[h][w].rgbtGreen = result[1];
            image[h][w].rgbtBlue = result[2];
        }
    }
    return;
}


// Return corressponding color value when given particular input
int color(int height, int width, RGBTRIPLE image[height][width], int xIndex, int yIndex, int color)
{
    switch (color)
    {
        case 0:
            return image[xIndex][yIndex].rgbtRed;
        case 1:
            return image[xIndex][yIndex].rgbtGreen;
        case 2:
            return image[xIndex][yIndex].rgbtBlue;
    }
    return 1;
}