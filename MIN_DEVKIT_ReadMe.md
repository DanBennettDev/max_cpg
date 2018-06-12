# Min-DevKit
[![Build Status](https://travis-ci.com/Cycling74/min-devkit.svg?token=GAmnsUEo9aYasSF5pz8q&branch=master)](https://travis-ci.com/Cycling74/min-devkit)
[![Build status](https://ci.appveyor.com/api/projects/status/0koqc3l3qyfu0l8b/branch/master?svg=true)](https://ci.appveyor.com/project/c74/min-devkit/branch/master)


Tools, documentation, and reference implementation of a Max Package built using the Min-API.


## Structure

There are two layers of material included in the Min-DevKit package. 

* `min-devkit` is an example package following current best-practices for package creation.
* `min-api` is a folder within the devkit containing all of the support files you will need to compile an external object written in modern C++.  This folder you will include in your own package's source folder.
* `min-lib` contains building blocks, helper classes, and unit generators that may be useful in authoring  C++ code for audio, video, and data processing.


## Prerequisites

To build the externals in this package you will need some form of compiler support on your system. 

* On the Mac this means **Xcode 7 or later** (you can get from the App Store for free). 
* On Windows this means **Visual Studio 2015 or 2017** (you can download a free version from Microsoft). The installer for Visual Studio 2017 offers an option to install Git, which you should choose to do.

You will also need to install a recent version of [CMake](https://cmake.org/download/).


## Building

0. Clone the *Min-DevKit* from Github (or download from inside of Max using the Package Manager). If you clone it from Github, **you should clone it into Max's Packages folder**. If you clone it elsewhere you will need to make an alias to it in your Packages folder.
   The *Packages* folder can be found inside of your *Max 7* or *Max 8* folder that inside of your user's *Documents* folder.
1. In the Terminal or Console app of your choice, change directories (cd) into the Min-Devkit folder you cloned/installed in step 0.
2. `mkdir build` to create a folder with your various build files
3. `cd build` to put yourself into that folder
4. Now you can generate the projects for your choosen build environment:

### Mac 

Run `cmake -G Xcode ..` and then run `cmake --build .` or open the Xcode project from this "build" folder and use the GUI.

Note: you can add the `-j4` option where "4" is the number of cores to use.  This can help to speed up your builds, though sometimes the error output is interleaved in such a way as to make troubleshooting more difficult.

### Windows

You can run `cmake --help` to get a list of the options available.  Assuming some version of Visual Studio 2017, the commands to generate the projects will look like this:

* 32 bit: `cmake -G "Visual Studio 15 2017" ..`
* 64 bit: `cmake -G "Visual Studio 15 2017 Win64" ..`

Having generated the projects, you can now build by opening the .sln file in the build folder with the Visual Studio app (just double-click the .sln file) or you can build on the command line like this:

`cmake --build . --config Release`


## Unit Testing

On the command like you can run all unit tests using Cmake:

* on debug builds: `ctest -C Debug .`
* on release builds: `ctest -C Release .`

Or you can run an individual test, which is simply a command line program:

* `cd ..`
* `cd tests`
* mac example: `./test_dcblocker_tilde -s`
* win example: `test_dcblocker_tilde.exe -s`

Or run them your IDE's debugger.


## Continuous Integration

Continuous Integration (CI) is...

The Min-DevKit project models CI using two different services, both of which are free and very easy to set up if your project is hosted publicly on Github.

* **Mac**: go to http://travis-ci.org and sign up.  If your repository follows the same model as Min-DevKit, you can copy `.travis.yml` directly with no changes.  Now every push to your repository will trigger an automatic build for the Mac with Travis CI.
* **Windows**: go to http://appveyor.com and sign up.  If your repository follows the same model as Min-DevKit, you can copy `appveyor.yml` directly with no changes.  Now every push to your repository will trigger automatic builds for both 32 and 64-bit Windows with Appveyor.

#### Private Dependencies

At this time the Min API is a private Github repository. As such there are additional challenges (authentication) that will not be present after the API is released. There are (at least) two paths to authentication for the private repositories:

* If you are creating a C74-owned package, ask Tim to add the `github_rsa_ci` private key to the relevant Travis project.
* If you are anyone else, generate an SSH key. Upload it to a Github account that has access to the Min-API. Then add the private key to the Travis project.

Yes, this is a pain, but you only have to do it once...

### Fetching your builds

* **Windows**: On Appveyor it is simple.  You go to the latest build, choose the "Platform" and then look under the "Artifacts" tab.
* **Mac**: Travis CI does not host your build files for you like Appveyor. Instead, you need to configure a place for Travis to upload your builds. This is most easily done by signing up for a [free Amazon Web Services account](http://aws.amazon.com/free/).

#### S3 Integration

The builds from Travis for this package can be found on [this S3-hosted page](https://s3-us-west-2.amazonaws.com/cycling74-ci/index.html?prefix=min-devkit/).

To setup your own page:

0. Sign up for a free AWS account as mentioned above
1. Create a "bucket" for your Travis builds
2. Add the correct info to your `.travis.yml` file for your S3 bucket as described in the [Travis S3 Documentation](https://docs.travis-ci.com/user/deployment/s3).
3. At this point, builds should go from Travis to a folder in your S3 bucket.
4. To access the build, you can log in to your S3 account and download it. Alternatively you may want to make your builds publically available. If you wish to do the later, read on...
5. upload the files from this folder's `source/min-api/max-api/site` folder to your S3 bucket's root folder. For more information on the content of these files, please visit the Github page for the [S3 Bucket Listing Code](https://github.com/rgrp/s3-bucket-listing).
6. As described on the above Github page, you will need to set the permissions for your bucket such that "Everyone" has both `listing` and `viewing` access.


## Additional Documentation

* [How to Create a New Package](./HowTo-NewPackage.md)
* [How to Create a New Object](./HowTo-NewObject.md)
* [How to Contribute to Min](./HowTo-Contribute.md)
* [Guide To Writing Objects](https://github.com/Cycling74/min-api/blob/master/doc/GuideToWritingObjects.md)
* [Min Wiki](https://github.com/Cycling74/min-devkit/wiki) For additional topics, advanced configuration, and user submissions



## Contributors / Acknowledgements

* See the [GitHub Contributor Graph](https://github.com/Cycling74/min-api/graphs/contributors) for the API
* See the [GitHub Contributor Graph](https://github.com/Cycling74/min-devkit/graphs/contributors) for the DevKit

## Support

For support, please use the developer forums at:
http://cycling74.com/forums/
