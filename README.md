dkv-cpp
=======

Distributed key value storage in cpp

## Build
You have to download my other two repos to use build dkv. They are
  * [common](https://github.com/allenbo/common.git)
  * [networking](https://github.com/allenbo/networking.git)

Please follow the steps below to build dkv

# make a workspace directory
cd ~ && mkdir workspace && cd workspace
# clone common and networking repos
git clone git@github.com:allenbo/common.git
git clone git@github.com:allenbo/networking.git
#compile networking
cd networking && make && cd ..
#compile dkv
cd dkv && make
