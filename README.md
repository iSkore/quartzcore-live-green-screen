# QuartzCore-Live-Green-Screen
Live streaming video of chroma key green screen cut out

<img src="https://s3.amazonaws.com/cerenity/SOResources/greenscreen.png" width="400" height="400" alt="green screen" />


### `greenScreen sample`

With green image billboard layering, cancel out images based on layer position

A Core Image kernel routine that computes a multiply effect.
The code looks up the source pixel in the sampler and then multiplies it by the value passed to the routine.

	kernel vec4 greenScreen( sampler image, __number select )
	{
		vec4 color = sample( image, samplerCoord( image ) );
		
		// HIGHLY SELECTIVE
		color.a = ( color.g <= 0.35 && color.g >= 0.2 && color.r <= 0.2 && color.b <= 0.275 && color.b >= 0.05 ) ? 0.0 : 1.0;
		
		// MID SELECTIVE
		color.a = ( color.r <= 0.175 && color.g <= 0.35 && color.g >= 0.15 && color.b <= 0.25 && color.b >= 0.05 ) ? 0.0 : 1.0;
		
		// LESS SELECTIVE
		color.a = ( color.r <= 0.155 && color.r >= 0.0 && color.g <= 0.35 && color.g >= 0.135 && color.b <= 0.2175 && color.b >= 0.0375 ) ? 0.0 : 1.0;
		
		return color;
	}


### DYNAMIC VARIABLES

1. **Vec (vector):**

	Creation: `new Vec(number_0, number_1, ...)`, take 2 to 4 numerical arguments, or `new Vec(vec)` (does a copy)

	Properties: `x = r = 0`, `y = g = 1`, `z = b = 2 = width`, `w = a = 3 = height`
<br/><br/>
2. **Shape (wrapper over CIFilterShape)**

	Creation: `new Shape(number_0, number_1, number_2, number_3)` or `new Shape(shape)` (does a copy)

	Properties (all functions return a new Shape):
	- `function intersect(shape)`
	- `function union(shape)`
	- `function inset(number_x, number_y)`
	- `function translate(number_0, number_1)`
	- `function scale(number_0, number_1)`
	- `function rotate(number_angle)`
<br/><br/>
3. **AffineTransform (wrapper over NSAffineTransform):**

	Creation: `new AffineTransform()` or `new AffineTransform(transform)` (does a copy)

	Properties returning a new transform:
	- `function scale(scaleXY) or scale(scaleX, scaleY)`
	- `function translate(tX, tY)`
	- `function invert()`
	- `function rotateByDegrees(angle)`
	- `function rotateByRadians(angle)`
	- `function appendTransform(transform)`
	- `function prependTransform(transform)`
	- `function setTransformStruct(object)` - object has properties m11, m12, ... of NSAffineTransformStruct

	Properties returning a vector of size 2:
	- `function transformPoint(vector_size_2)`
	- `function transformSize(vector_size_2)`

	Property returning an JavaScript object:
	- `property transformStruct` - an object with properties m11, m12, ... of NSAffineTransformStruct

	Settable property:
	- `property transformStruct` - takes an object with properties m11, m12, ... of NSAffineTransformStruct
<br/><br/></br>
4. **Image:**

	Creation: Not allowed.

	Properties: extent (a Vec of dimension 4), definition (a Shape).

5. **Kernel:**

	Creation: Not allowed. The kernels defined in the kernel window are automatically set in the JavaScript context as variables in the kernel name.

	Properties:
	- `function apply(DOD, ROI_userInfo, arg_1, arg_2, ...)`
		* DOD: A Vec of dimension 4, a Shape or "null" for infinite image
		* ROI_userInfo: An object that will be passed to the ROI function
		* arg_x: Arguments given to the kernel
	- `ROIHandler`
		* To set the ROI function
		* The ROI function, when called, will receive 3 arguments:
		  samplerIndex (an int), dstRect (a Vec of dim 4), and info (passed to the apply function)  
		Example:
		        function myROIFunction(samplerIndex, dstRect, info) {
					return dstRect;
				}
				myKernel.ROIHandler = myROIFunction;

6. **ImageAccumulator (wrapper over CIImageAccumulator):**

	Creation: `new ImageAccumulator(extent, format)`. Extent is a `Vec` of dimension 4, format is "ARGB8", "RGBA16" or "RGBAf".

Property returning a `Vec` of dimension 4:
- property extent

Property returning a string:
- property format
Property returning an image:
- property image
Settable property:
- function setImage(image)
- function setImage(image, replacingRect)

### STATIC VARIABLES

**Filter:**

Properties:

 - All Core Image filter under a real (e.g. CISepiaTone) or simplied name (e.g. sepiaTone)
 - Returns a function to apply that filter.
 - The arguments are taken in the order defined at:
     - http://developer.apple.com/documentation/GraphicsImaging/Reference/CoreImageFilterReference

Example:

	var sepiaImage = Filter.sepiaTone(image, 1.0);

### FUNCTION TO IMPLEMENT

`function [return_type] main([arg_type_0] arg_0, [arg_type_1] arg_1, ...)`

`[arg_type_x]` can be:  `__image, __vec2, __vec3, __vec4, __color, __number, __index`

`[return_type]` must be: `__image`
Must return an image.

Example of the "main" function associated to the first kernel in the
kernel window and current parameters has been generated below:

	function __image main(__image image) {
		return greenScreen.apply( image.definition, null, image );
	}

### Green screen averaging and selecting

 - **AVERAGES**
     - FULL RGB
         - `r = 80`
         - `g = 142`
         - `b = 108`
     - Cocoa traslation
         - `r = 0.31372549`
         - `g = 0.55686275`
         - `b = 0.07058824`

 - **High level selectivity**
     - **RED:** `x ≤ 0.2`
     - **GREEN:** `0.2 ≤ x ≤ 0.35`
     - **BLUE:** `0.05 ≤ x ≤ 0.275`

 - **Mid level selectivity**
     - **RED:** `x ≤ 0.175`
     - **GREEN:** `0.15 ≤ x ≤ 0.35`
     - **BLUE:** `0.05 ≤ x ≤ 0.25`

 - **Low level selectivity**
     - **RED:** `x ≤ 0.155`
     - **GREEN:** `0.135 ≤ x ≤ 0.35`
     - **BLUE:** `0.0375 ≤ x ≤ 0.2175`

### ASCII Reactivity

 - Audio Input
     - Speaking sensitivity
         - `Audio Input`
             - Input Params
                 - Increasing Scale: `1`
                 - Decreasing Scale: `0.25`
         - `JavaScript`
             - Output: `0.5 - inputNumber`

     - Classroom sensitivity:
         - `Audio Input`
             - Input Params
                 - Increasing Scale: `1.5`
                 - Decreasing Scale: `0.5`
         - `JavaScript`
             - Output: `1.0 - inputNumber`

     - Jet engine sensitivity:
         - `Audio Input`
		     - Input Params
		         - Increasing Scale: `5`
		         - Decreasing Scale: `2.5`
         - `JavaScript`
             - Output: `5.0 - inputNumber`





>To do:

	VideoInput => ASCII w/audio in (0.322) => Billboard
	Host Info => CPU Load => Image W/ String => Billboard