echo "-- format files"
clang-format --style=file:.clang-format -i components/*/*.h components/*/*.cpp components/*/*/*.h components/*/*/*.cpp
cpplint --filter=-legal/copyright,-build/include_subdir --recursive components/ 
echo "-- formatted"