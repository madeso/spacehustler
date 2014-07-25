// Euphoria - Copyright (c) Gustav

#include "euphoria/mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <sstream>

#include "euphoria/str.h"

namespace {
const unsigned int kAssimpFlags =
    aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType |
    aiProcess_FlipUVs | aiProcess_GenUVCoords | aiProcess_TransformUVCoords |
    aiProcess_OptimizeMeshes | aiProcess_RemoveRedundantMaterials |
    aiProcess_PreTransformVertices | aiProcess_ImproveCacheLocality |
    aiProcess_FindDegenerates | aiProcess_JoinIdenticalVertices |
    aiProcess_ValidateDataStructure | aiProcess_GenSmoothNormals |
    aiProcess_FindInvalidData;

Texture::WrapMode GetTextureWrappingMode(const int mode) {
  switch (mode) {
    case aiTextureMapMode_Wrap:
      return Texture::kWrap_Repeat;
    case aiTextureMapMode_Clamp:
      return Texture::kWrap_ClampToEdge;
    case aiTextureMapMode_Decal:
      throw "Unsupported texture wrapping mode: decal";
    case aiTextureMapMode_Mirror:
      return Texture::kWrap_MirrorRepeat;
    default:
      throw "Unhandled texture wrapping mode";
  }
}

void AddMaterials(Mesh* ret, const aiScene* scene) {
  for (unsigned int material_id = 0; material_id < scene->mNumMaterials;
       ++material_id) {
    const aiMaterial* mat = scene->mMaterials[material_id];

    internal::Material material;

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) <= 0) {
      /*
      const std::string error = Str() << "Missing texture"; throw error;
      */
      /// @todo change to a better path, possible configurable or a auto
      /// generated one
      material.texture = "hazard.png";
    } else {
      aiString texture;
      mat->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
      material.texture = texture.C_Str();
    }

    int u = 0;
    int v = 0;
    mat->Get(AI_MATKEY_MAPPINGMODE_U(aiTextureType_DIFFUSE, 0), u);
    mat->Get(AI_MATKEY_MAPPINGMODE_V(aiTextureType_DIFFUSE, 0), v);
    material.wraps = GetTextureWrappingMode(u);
    material.wrapt = GetTextureWrappingMode(v);
    ret->materials.push_back(material);
  }
}

void AddFaces(internal::MeshPart* part, const aiMesh* mesh) {
  for (unsigned int face_id = 0; face_id < mesh->mNumFaces; ++face_id) {
    const aiFace& face = mesh->mFaces[face_id];
    part->AddFace(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
  }
}

void AddPoints(internal::MeshPart* part, const aiMesh* mesh) {
  for (unsigned int index = 0; index < mesh->mNumVertices; ++index) {
    const aiVector3D& vertex = mesh->mVertices[index];
    float u = 0;
    float v = 0;
    if (mesh->HasTextureCoords(0)) {
      const aiVector3D uv = mesh->mTextureCoords[0][index];
      u = uv.x;
      v = uv.y;
    }
    part->AddPoint(vertex.x, vertex.y, vertex.z, u, v);
  }
}

internal::MeshPart ConvertMesh(const aiMesh* mesh) {
  internal::MeshPart part;

  part.material = mesh->mMaterialIndex;
  AddPoints(&part, mesh);
  AddFaces(&part, mesh);

  return part;
}

Mesh ConvertScene(const aiScene* scene) {
  Mesh ret;

  /** @todo add parsing of nodes to the mesh so we could
  dynamically animate some rotors, wings etc. for example
   */

  if (scene->HasMeshes() == false) {
    const std::string error = Str() << "Scene is missing meshes";
    throw error;
  }

  AddMaterials(&ret, scene);

  for (unsigned int meshid = 0; meshid < scene->mNumMeshes; ++meshid) {
    const aiMesh* mesh = scene->mMeshes[meshid];
    const internal::MeshPart part = ConvertMesh(mesh);
    ret.parts.push_back(part);
  }
  return ret;
}

// http://assimp.sourceforge.net/howtoBasicShapes.html
Mesh LoadFromString(const std::string& nff, const std::string& format) {
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFileFromMemory(
      nff.c_str(), nff.length() + 1, kAssimpFlags, format.c_str());
  if (!scene) {
    throw importer.GetErrorString();
  }
  return ConvertScene(scene);
}

const std::string kFormatNff = "nff";
const std::string kFormatObj = "obj";

}  // namespace

Mesh LoadMesh(const std::string& path) {
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(path, kAssimpFlags);
  if (!scene) {
    throw importer.GetErrorString();
  }
  return ConvertScene(scene);
}

Mesh CreateCube(float size) {
  std::ostringstream ss;
  ss << "shader texture.png" << std::endl << "hex 0 0 0 " << size;
  return LoadFromString(ss.str(), kFormatNff);
}

Mesh CreateSphere(float size, const std::string& texture) {
  std::ostringstream ss;
  ss << "shader " << texture << std::endl << "s 0 0 0 " << size;
  return LoadFromString(ss.str(), kFormatNff);
}

Mesh CreateBox(float width, float height, float depth) {
  const float x = width / 2;
  const float y = height / 2;
  const float z = depth / 2;
  std::ostringstream ss;
  ss << "v " << -x << " "
     << " " << -y << " " << -z << std::endl << "v " << -x << " "
     << " " << -y << " " << z << std::endl << "v " << -x << " "
     << " " << y << " " << -z << std::endl << "v " << -x << " "
     << " " << y << " " << z << std::endl << "v " << x << " "
     << " " << -y << " " << -z << std::endl << "v " << x << " "
     << " " << -y << " " << z << std::endl << "v " << x << " "
     << " " << y << " " << -z << std::endl << "v " << x << " "
     << " " << y << " " << z << std::endl << "" << std::endl << "vn 0 0 -1"
     << std::endl << "vn 0 0 1" << std::endl << "vn -1 0 0" << std::endl
     << "vn 1 0 0" << std::endl << "vn 0 1 0" << std::endl << "vn 0 -1 0"
     << std::endl << "vn 0 0 -1" << std::endl << "vn 0 -1 0" << std::endl
     << "vn -1 0 0" << std::endl << "vn 0 0 1" << std::endl << "vn 0 -1 0"
     << std::endl << "vn -1 0 0" << std::endl << "vn 0 0 -1" << std::endl
     << "vn 0 1 0" << std::endl << "vn -1 0 0" << std::endl << "vn 0 0 1"
     << std::endl << "vn 0 1 0" << std::endl << "vn -1 0 0" << std::endl
     << "vn 0 0 -1" << std::endl << "vn 0 -1 0" << std::endl << "vn 1 0 0"
     << std::endl << "vn 0 0 1" << std::endl << "vn 0 -1 0" << std::endl
     << "vn 1 0 0" << std::endl << "vn 0 0 -1" << std::endl << "vn 0 1 0"
     << std::endl << "vn 1 0 0" << std::endl << "vn 0 0 1" << std::endl
     << "vn 0 1 0" << std::endl << "vn 1 0 0" << std::endl << "" << std::endl
     << "vt 0 0" << std::endl << "vt 0 1" << std::endl << "vt 1 1" << std::endl
     << "vt 1 0" << std::endl << "" << std::endl
     << "f 3/1/13 7/2/25 5/3/19 1/4/7" << std::endl
     << "f 6/1/22 8/2/28 4/3/16 2/4/10" << std::endl
     << "f 2/1/12 4/2/18 3/3/15 1/4/9" << std::endl
     << "f 7/1/27 8/2/30 6/3/24 5/4/21" << std::endl
     << "f 4/1/17 8/2/29 7/3/26 3/4/14" << std::endl
     << "f 5/1/20 6/2/23 2/3/11 1/4/8" << std::endl;

  return LoadFromString(ss.str(), kFormatObj);
}
