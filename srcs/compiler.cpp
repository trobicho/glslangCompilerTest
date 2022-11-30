#include "compiler.hpp"
#include <iostream>
#include <stdexcept>

GlslCompiler::GlslCompiler() {
  if (!glslang::InitializeProcess())
    throw std::runtime_error("Error with InitializeProcess");
}

GlslCompiler::~GlslCompiler() {
  glslang::FinalizeProcess();
}
  
void  GlslCompiler::setShader(const std::string &filename) {
  m_shaderIfs = std::ifstream(filename, std::ifstream::in);
  if (!m_shaderIfs.is_open())
    throw std::invalid_argument("Cannot open file: " + filename);

  std::string filenameWithoutPath = filename;
  auto posBegin = filename.find_last_of('/');
  if (posBegin != std::string::npos)
    filenameWithoutPath = filename.substr(posBegin + 1);

  auto pos = filenameWithoutPath.find_last_of('.');
  if (pos == std::string::npos)
    throw std::invalid_argument("cannot deduce shader stage from filename: " + filenameWithoutPath);
  std::string stage = filenameWithoutPath.substr(pos + 1);

  if (stage == "vert") //for a vertex shader
    m_stage = EShLanguage::EShLangVertex;
  else if (stage == "frag") //for a fragment shader
    m_stage = EShLanguage::EShLangFragment;
  else if (stage == "comp") //for a compute shader
    m_stage = EShLanguage::EShLangCompute;
  else if (stage == "rgen ") //for a ray generation shader
    m_stage = EShLanguage::EShLangRayGen;
  else if (stage == "rint ") //for a ray intersection shader
    m_stage = EShLanguage::EShLangIntersect;
  else if (stage == "rahit") //for a ray any hit shader
    m_stage = EShLanguage::EShLangAnyHit;
  else if (stage == "rchit") //for a ray closest hit shader
    m_stage = EShLanguage::EShLangClosestHit;
  else if (stage == "rmiss") //for a ray miss shader
    m_stage = EShLanguage::EShLangMiss;
  else if (stage == "rcall") //for a ray callable shader
    m_stage = EShLanguage::EShLangCallable;
  else if (stage == "tesc") //for a tessellation control shader
    m_stage = EShLanguage::EShLangTessControl;
  else if (stage == "tese") //for a tessellation evaluation shader
    m_stage = EShLanguage::EShLangTessEvaluation;
  else if (stage == "geom") //for a geometry shader
    m_stage = EShLanguage::EShLangGeometry;
  else if (stage == "mesh") //for a mesh shader
    m_stage = EShLanguage::EShLangMeshNV;
  else if (stage == "task") //for a task shader
    m_stage = EShLanguage::EShLangTaskNV;
  else
    throw std::invalid_argument("invalid stage: " + stage);

  std::cout << "valid stage: " << stage << std::endl;
}

void  GlslCompiler::shaderValidate() {
  if (!m_shaderIfs.is_open()) {
    throw std::invalid_argument("shader as not been set");
  }

  glslang::TShader  shader = glslang::TShader(m_stage);
  shader.setEnvInput(glslang::EShSourceGlsl, m_stage, glslang::EShClientVulkan, 0);
  shader.setEnvClient(glslang::EShClientNone, glslang::EShTargetClientVersion(0));
  shader.setEnvTarget(glslang::EShTargetNone, glslang::EShTargetLanguageVersion(0));

  std::vector<std::string> lines;
  for (std::string line; std::getline(m_shaderIfs, line); ) {
    lines.push_back(line);
  }
  std::vector<const char *> lines_cstr;
  lines_cstr.reserve(lines.size());
  for (auto& line : lines) {
    lines_cstr.push_back(line.c_str());
  }
  shader.setStrings(lines_cstr.data(), lines.size());

  EShMessages       messages = EShMsgDefault;
  TBuiltInResource  builtInResources;

  bool  parseRet = shader.parse(&builtInResources, 0, true, messages);
  if (!parseRet) {
    std::cout << "parsing failed" << std::endl;
  }
  else {
    std::cout << "parsing successed" << std::endl;
  }
  std::cout << std::endl;
  std::cout << "INFO LOG:" << std::endl;
  std::cout << shader.getInfoLog() << std::endl;
  std::cout << std::endl;

  std::cout << "INFO DEBUG LOG:" << std::endl;
  std::cout << shader.getInfoDebugLog() << std::endl;
  std::cout << std::endl;

  std::cout << "INTERMEDIATE:" << std::endl;
  std::cout << shader.getIntermediate() << std::endl;
}
