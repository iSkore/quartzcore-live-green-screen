kernel vec4 ASCII( sampler image, sampler text, vec2 charSize, float numCols, float numRows, float numChars, float override, __color newColor )
{
	vec2 dC = destCoord();
	vec2 intraCharCoord = mod( dC, charSize );
	// get the value at the center of this cell
	vec2 centerPoint = dC - intraCharCoord + charSize / 2.;
	vec4 centerValue = sample( image, samplerTransform( image, centerPoint ) );
	
	// compute the luminance
	float luminance = ( centerValue.r + centerValue.g + centerValue.b ) / 3.;
	luminance = clamp( luminance, 0., 1. );
	
	// get the corresponding fontImage pixel coord
	float charIndex = floor( luminance * ( numChars-1. ) );
	float charCol = mod( charIndex, numCols );
	float charRow = floor( charIndex / numCols );
	
	vec2 charPos = vec2( charCol, charRow );
	vec2 charOrigin = charPos * ( charSize );
	vec2 textCoord = charOrigin + intraCharCoord;
	
	// get the color for the character
	// this will either be a 
	vec4 c = ( 1.-override ) * centerValue + override * newColor;
	
	return sample( text, samplerTransform( text, textCoord ) ) * c;
}

function myROIFunction( samplerIndex, dstRect, info ) { 
	var roiRect = dstRect;
	// inputImage : ROI is approximate and contains all possible pixels which could be the center pixel
	if( samplerIndex == 0 ) {
		roiRect.x -= info.charSize[ 0 ] / 2.;
		roiRect.y -= info.charSize[ 1 ] / 2.;
		roiRect.width += info.charSize[ 0 ];
		roiRect.height += info.charSize[ 1 ];
	}
	// fontImage : ROI contains the entire fontImage
	if( samplerIndex == 1 ) {
		roiRect = info.fontImageExtent;
	}
	
	return roiRect;
}

ASCII.ROIHandler = myROIFunction;

function __image main( __image image, __image text, __vec2 charSize, __number numCols, __number numRows, __number numChars, __number override, __color newColor ) {
	var info = new Object();
	info.charSize = charSize;
	info.fontImageExtent = text.extent;
	
	return ASCII.apply( image.definition, info, image, text, charSize, numCols, numRows, numChars, override, newColor );
}