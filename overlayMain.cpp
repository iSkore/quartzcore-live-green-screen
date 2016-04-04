function __image main(__image image) {
	return darkToTransparent.apply( image.definition, null, image );
}
