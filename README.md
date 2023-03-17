# CSxD
Counter-Strike xD

# Build & Run

```sh
sudo apt install -y build-essential cmake googletest google-mock libgtest-dev libgmock-dev nlohmann-json3-dev
git clone https://github.com/SMModarresy/CSxD.git
cd CSxD
cmake CMakeLists.txt
make
cd src
./CSxD
```
If you're using CLion, make sure to set the working directory to `$PROJECT_DIR$/src` for the json file to be loaded

# UML
![UML Diagram](CSxD.drawio.svg)
