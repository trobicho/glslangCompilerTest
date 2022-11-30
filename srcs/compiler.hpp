#pragma once
#include <glslang/Public/ShaderLang.h>
#include <fstream>

class GlslCompiler {
  public:
    GlslCompiler();
    ~GlslCompiler();

    void  setShader(const std::string &filename);

    void  shaderValidate();

  private:
    std::ifstream     m_shaderIfs;
    EShLanguage       m_stage = EShLanguage::EShLangVertex;
};
