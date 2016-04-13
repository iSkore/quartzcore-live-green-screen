/*
A Core Image kernel routine that computes a multiply effect.
The code looks up the source pixel in the sampler and then multiplies it by the value passed to the routine.
*/

kernel vec4 darkToTransparent(sampler image)
{
	// AVERAGES:
	//       FULL RGB = 80, 142, 108
	//       Cocoa traslation:
	//		    r = 0.31372549
	//		    g = 0.55686275
	//		    b = 0.07058824

	vec4 color = sample(image, samplerCoord(image));

	// HIGHLY SELECTIVE
	color.a = ( color.g <= 0.35 && color.g >= 0.2 && color.r <= 0.2 && color.b <= 0.275 && color.b >= 0.05 ) ? 0.0 : 1.0;
	
	// MID SELECTIVE
	color.a = ( color.r <= 0.175 && color.g <= 0.35 && color.g >= 0.15 && color.b <= 0.25 && color.b >= 0.05 ) ? 0.0 : 1.0;
	
	// LESS SELECTIVE
	color.a = ( color.r <= 0.155 && color.r >= 0.0 && color.g <= 0.35 && color.g >= 0.135 && color.b <= 0.2175 && color.b >= 0.0375 ) ? 0.0 : 1.0;

	return color;
}

kernel vec4 coreImageKernel(sampler image, __color color)
{
	return sample(image, samplerCoord(image)) * color;
}
