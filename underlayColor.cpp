kernel vec4 darkToTransparent(sampler image)
{
	vec4 color = sample( image, samplerCoord( image ) );

	color.r = 1.0;
	//color.g = ( color.g <= 0.4 && color.r <= 0.3 && color.b <= 0.09 ) ? 1.0 : 0.0;
	color.g = 0.0;
	color.b = 0.0;
	color.a = 1.0;

	return color;
}

kernel vec4 coreImageKernel( sampler image ) {
	vec4 color = sample( image, samplerCoord( image ) );

	return color;
}
