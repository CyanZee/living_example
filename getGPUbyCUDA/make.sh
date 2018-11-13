
# g++ -o main main.cpp -I /usr/local/cuda/include -L /usr/local/cuda/lib64 -std=c++11 -lcudart

#g++ -o main getGPUMain.cpp -std=c++11 -ldl -lnvidia-ml -I /usr/local/cuda/include/ -L /usr/local/cuda/lib64/stubs/

g++ -o main getGPUMain.cpp -std=c++11 -lnvidia-ml -I /usr/local/cuda/include/ 
