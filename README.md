# Picopad Playground

**Clone repository**

```bash
  git clone https://github.com/tvecera/picopad-playground
```

## PyPicoPadImg Generator

This Python program is designed to generate image arrays compatible with Picopad SDK. It takes an image file as input and 
creates a C array of pixel values, ready to be used in your Picopad SDK software. The program utilizes the 
Run-Length Encoding (RLE) compression technique to reduce the memory footprint of the images.

Please note that this software currently only tested on 8-bit PNG images using a color lookup table (palette).

The code of this generator is based on the original Picopad SDK source code.

## PyConvertLoaderImg

PyConvertLoaderImg is a Python-based tool that is capable of converting images into 16-bit BMP images. This tool
specifically generates images that are compatible with the Picopad loader application.

The tool reads image files, provides some basic information about them, downsamples the images by reducing the bit depth
of each color channel, and saves the resulting image as a 16-bit BMP file.

## Picopad TFT Display emulator

This project provides an emulator for the TFT display of Picopad handheld devices. It's designed to assist in the
prototyping and development of applications for Picopad handhelds. With this tool, you can easily prepare and test your
display code using the API of the Picopad SDK.

The code of this emulator is based on the original Picopad SDK source code.

## Custom Picopad Loader

Fork of the Picopad SDK, built on the Pico SDK. The goal was to add the capability to use libraries for the Pico SDK 
on the Picopad.

## Links

- Picopad from Pajenicko e-shop
- Picopad SDK (https://github.com/Pajenicko/Picopad)
