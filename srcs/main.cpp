#include "compiler.hpp"
#include <iostream>

int main(int ac, char **av) {
  GlslCompiler  compiler;

  if (ac > 1) {
    compiler.setShader(av[1]);
  }
  else {
    std::cout << "Usage ./glslCompiler [FILE]" << std::endl;
  }
  return (EXIT_SUCCESS);
}
