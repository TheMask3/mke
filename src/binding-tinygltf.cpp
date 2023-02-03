#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "binding-tinygltf.h"

VALUE rb_mTinyGLTF,
      rb_eTinyGLTFError,
      rb_cModel,
      rb_cAccessor,
      rb_cAsset,
      rb_cAnimation,
      rb_cAnimationChannel,
      rb_cAnimationSampler,
      rb_cBuffer,
      rb_cBufferView,
      rb_cMaterial,
      rb_cMesh,
      rb_cNode,
      rb_cPrimitive,
      rb_cTexture,
      rb_cImage,
      rb_cSkin,
      rb_cSampler,
      rb_cCamera,
      rb_cScene,
      rb_cLight;

/* call-seq: GLTF.load(filename_or_data[, base_dir]) => new Model
 *
 * Loads a model from the given file or data string. The data string can be
 * ASCII or binary GLTF. `base_dir` is used to look up external dependencies
 * such as images. If `base_dir` is omitted, it defaults to either the
 * directory containing the file (if loading a file), or `'.'` (if loading a
 * data string).
 *
 * Returns the new model, or raises an error if the model could not be loaded.
 */
VALUE rb_tgltf_load(int argc, VALUE *argv, VALUE self) {
  TinyGLTF gltf;
  VALUE rstr, rbase_dir;
  rb_scan_args(argc, argv, "11", &rstr, &rbase_dir);
  VALUE is_file = rb_funcall(rb_cFile, rb_intern("exist?"), 1, rstr);

  if (NIL_P(rbase_dir)) {
    if (is_file == Qtrue) {
      rbase_dir = rb_funcall(rb_cFile, rb_intern("dirname"), 1, rstr);
    } else {
      rbase_dir = rb_str_new2(".");
    }
  }

  if (is_file) {
    VALUE rfile = rb_funcall(rb_cFile, rb_intern("open"), 2, rstr, rb_str_new2("rb"));
    rstr = rb_funcall(rfile, rb_intern("read"), 0);
    rb_funcall(rfile, rb_intern("close"), 0);
  }

  try {
    Model model;
    std::string err, warn, base_dir(StringValueCStr(rbase_dir));
    bool res;
    if (RSTRING_LEN(rstr) > 0 && *RSTRING_PTR(rstr) == 'g') { // 'glTF'
      res = gltf.LoadBinaryFromMemory(&model, &err, &warn,
                                      (unsigned char *) RSTRING_PTR(rstr), (unsigned int) RSTRING_LEN(rstr), base_dir);
    } else {
      res = gltf.LoadASCIIFromString( &model, &err, &warn,
                                      RSTRING_PTR(rstr), (unsigned int) RSTRING_LEN(rstr), base_dir);
    }
    if (err.size()  > 0) rb_raise(rb_eTinyGLTFError, "%s", err.c_str());
    if (warn.size() > 0) rb_raise(rb_eTinyGLTFError, "%s", warn.c_str());
    if (!res) rb_raise(rb_eTinyGLTFError, "unknown failure to load model");
    return rModel_new(&model);
  } catch(const std::exception &ex) {
    rb_raise(rb_eTinyGLTFError, "%s", ex.what());
  }
  return Qnil;
}

VALUE rAccessor_new(const Accessor *accessor, VALUE rmodel) {
  VALUE raccessor = rb_funcall(rb_cAccessor, rb_intern("new"), 0);
  *Accessor_unwrap(raccessor) = *accessor;

  rb_ivar_set(raccessor, rb_intern("@model"),             rmodel);
  rb_ivar_set(raccessor, rb_intern("@buffer_view_index"), RINDEX_OR_NIL(accessor->bufferView));
  rb_ivar_set(raccessor, rb_intern("@component_type"),    component_type_to_sym(accessor->componentType));
  rb_ivar_set(raccessor, rb_intern("@count"),             SIZET2NUM(accessor->count));
  rb_ivar_set(raccessor, rb_intern("@name"),              rb_str_new2(accessor->name.c_str()));
  rb_ivar_set(raccessor, rb_intern("@byte_offset"),       SIZET2NUM(accessor->byteOffset));
  rb_ivar_set(raccessor, rb_intern("@normalized"),        accessor->normalized ? Qtrue : Qfalse);
  rb_ivar_set(raccessor, rb_intern("@type"),              type_to_sym(accessor->type));
  rb_ivar_set(raccessor, rb_intern("@extras"),            rValue_new(&accessor->extras, rmodel));
  rb_ivar_set(raccessor, rb_intern("@min"),               Qnil);
  rb_ivar_set(raccessor, rb_intern("@max"),               Qnil);

  if (accessor->minValues.size() > 0) {
    VALUE ary = rb_ary_new();
    for (size_t i = 0; i < accessor->minValues.size(); i++)
      rb_ary_push(ary, DBL2NUM(accessor->minValues[i]));
    rb_ivar_set(raccessor, rb_intern("@min"), ary);
  }

  if (accessor->maxValues.size() > 0) {
    VALUE ary = rb_ary_new();
    for (size_t i = 0; i < accessor->maxValues.size(); i++)
      rb_ary_push(ary, DBL2NUM(accessor->maxValues[i]));
    rb_ivar_set(raccessor, rb_intern("@max"), ary);
  }

  return raccessor;
}

VALUE rAnimation_new(const Animation *animation, VALUE rmodel) {
  VALUE ranimation = rb_funcall(rb_cAnimation, rb_intern("new"), 0);
  // *Animation_unwrap(ranimation) = *animation;

  VALUE rchannels = rb_ary_new();
  for (size_t i = 0; i < animation->channels.size(); i++)
    rb_ary_push(rchannels, rAnimationChannel_new(&animation->channels[i], ranimation));

  VALUE rsamplers = rb_ary_new();
  for (size_t i = 0; i < animation->samplers.size(); i++)
    rb_ary_push(rsamplers, rAnimationSampler_new(&animation->samplers[i], ranimation));

  rb_ivar_set(ranimation, rb_intern("@model"),     rmodel);
  rb_ivar_set(ranimation, rb_intern("@name"),      rb_str_new2(animation->name.c_str()));
  rb_ivar_set(ranimation, rb_intern("@channels"),  rchannels);
  rb_ivar_set(ranimation, rb_intern("@samplers"),  rsamplers);
  rb_ivar_set(ranimation, rb_intern("@extras"),    rValue_new(&animation->extras, rmodel));

  return ranimation;
}

VALUE rAnimationChannel_new(const AnimationChannel *animation_channel, VALUE ranimation) {
  VALUE ranimation_channel = rb_funcall(rb_cAnimationChannel, rb_intern("new"), 0);
  // *AnimationChannel_unwrap(ranimation_channel) = *animation_channel;

  rb_ivar_set(ranimation_channel, rb_intern("@animation"),         ranimation);
  rb_ivar_set(ranimation_channel, rb_intern("@sampler_index"),     RINDEX_OR_NIL(animation_channel->sampler));
  rb_ivar_set(ranimation_channel, rb_intern("@target_node_index"), RINDEX_OR_NIL(animation_channel->target_node));
  rb_ivar_set(ranimation_channel, rb_intern("@target_path"),       ID2SYM(rb_intern(animation_channel->target_path.c_str())));
  rb_ivar_set(ranimation_channel, rb_intern("@extras"),            rValue_new(&animation_channel->extras, rb_funcall(ranimation, rb_intern("model"), 0)));

  return ranimation_channel;
}

VALUE rAnimationSampler_new(const AnimationSampler *animation_sampler, VALUE ranimation) {
  VALUE ranimation_sampler = rb_funcall(rb_cAnimationSampler, rb_intern("new"), 0);
  // *AnimationSampler_unwrap(ranimation_sampler) = *animation_sampler;

  std::string interp = animation_sampler->interpolation;
  std::transform(interp.begin(), interp.end(), interp.begin(), ::tolower);

  rb_ivar_set(ranimation_sampler, rb_intern("@animation"),     ranimation);
  rb_ivar_set(ranimation_sampler, rb_intern("@input_index"),   RINDEX_OR_NIL(animation_sampler->input));
  rb_ivar_set(ranimation_sampler, rb_intern("@output_index"),  RINDEX_OR_NIL(animation_sampler->output));
  rb_ivar_set(ranimation_sampler, rb_intern("@interpolation"), ID2SYM(rb_intern(interp.c_str())));
  rb_ivar_set(ranimation_sampler, rb_intern("@extras"),        rValue_new(&animation_sampler->extras, rb_funcall(ranimation, rb_intern("model"), 0)));

  return ranimation_sampler;
}

VALUE rAsset_new(const Asset *asset, VALUE rmodel) {
  VALUE rasset = rb_funcall(rb_cAsset, rb_intern("new"), 0);
  // *Asset_unwrap(rasset) = *asset;

  rb_ivar_set(rasset, rb_intern("@model"),       rmodel);
  rb_ivar_set(rasset, rb_intern("@version"),     rb_str_new2(asset->version.c_str()));
  rb_ivar_set(rasset, rb_intern("@generator"),   rb_str_new2(asset->generator.c_str()));
  rb_ivar_set(rasset, rb_intern("@min_version"), rb_str_new2(asset->minVersion.c_str()));
  rb_ivar_set(rasset, rb_intern("@copyright"),   rb_str_new2(asset->copyright.c_str()));
  rb_ivar_set(rasset, rb_intern("@extensions"),  rExtensionMap_new(&asset->extensions, rmodel));
  rb_ivar_set(rasset, rb_intern("@extras"),      rValue_new(&asset->extras, rmodel));
  
  return rasset;
}

VALUE rBuffer_new(const Buffer *buf, VALUE rmodel) {
  VALUE rbuf = rb_funcall(rb_cBuffer, rb_intern("new"), 0);

  // Keep (yes duplicate but the original will be freed) the buffer data
  // in memory w/o having to wrap with a ruby string, which can be expensive
  // and inefficient.
  Buffer *dup = Buffer_unwrap(rbuf);
  dup->data = buf->data;
  dup->uri  = buf->uri;

  rb_ivar_set(rbuf, rb_intern("@model"),  rmodel);
  rb_ivar_set(rbuf, rb_intern("@name"),   rb_str_new2(buf->name.c_str()));
  rb_ivar_set(rbuf, rb_intern("@extras"), rValue_new(&buf->extras, rmodel));

  return rbuf;
}

VALUE Buffer_to_ptr(VALUE self) {
  // return Fiddle::Pointer.new(addr, size)
  VALUE rFiddlePointer = rb_const_get(rb_const_get(rb_cObject, rb_intern("Fiddle")),
                                      rb_intern("Pointer"));
  Buffer *buf = Buffer_unwrap(self);
  return rb_funcall(rFiddlePointer, rb_intern("new"), 2,
                    ULL2NUM((unsigned long long) buf->data.data()),
                    ULL2NUM((unsigned long long) buf->data.size()));
}

VALUE Buffer_size(VALUE self) {
  return ULONG2NUM(Buffer_unwrap(self)->data.size());
}

VALUE Buffer_to_s(VALUE self) {
  Buffer *buf = Buffer_unwrap(self);
  return rb_str_new((char *) buf->data.data(), buf->data.size());
}

VALUE Buffer_uri(VALUE self) {
  Buffer *buf = Buffer_unwrap(self);
  if (buf->uri.size() > 0)
    return rb_str_new2(buf->uri.c_str());
  else
    return Qnil;
}

VALUE rBufferView_new(const BufferView *view, VALUE rmodel) {
  VALUE rbuf = rb_funcall(rb_cBufferView, rb_intern("new"), 0);
  *BufferView_unwrap(rbuf) = *view;

  rb_ivar_set(rbuf, rb_intern("@model"),        rmodel);
  rb_ivar_set(rbuf, rb_intern("@name"),         rb_str_new2(view->name.c_str()));
  rb_ivar_set(rbuf, rb_intern("@buffer_index"), RINDEX_OR_NIL(view->buffer));
  rb_ivar_set(rbuf, rb_intern("@byte_offset"),  SIZET2NUM(view->byteOffset));
  rb_ivar_set(rbuf, rb_intern("@byte_length"),  SIZET2NUM(view->byteLength));
  rb_ivar_set(rbuf, rb_intern("@byte_stride"),  SIZET2NUM(view->byteStride));
  rb_ivar_set(rbuf, rb_intern("@target"),       target_to_sym(view->target));
  rb_ivar_set(rbuf, rb_intern("@extras"),       rValue_new(&view->extras, rmodel));

  return rbuf;
}

VALUE rCamera_new(const Camera *camera, VALUE rmodel) {
  VALUE rcamera = rb_funcall(rb_cCamera, rb_intern("new"), 0);
  // *Camera_unwrap(rcamera) = *camera;

  rb_ivar_set(rcamera, rb_intern("@model"),      rmodel);
  rb_ivar_set(rcamera, rb_intern("@name"),       rb_str_new2(camera->name.c_str()));
  rb_ivar_set(rcamera, rb_intern("@type"),       rb_intern(camera->type.c_str()));
  rb_ivar_set(rcamera, rb_intern("@extensions"), rExtensionMap_new(&camera->extensions, rmodel));
  rb_ivar_set(rcamera, rb_intern("@extras"),     rValue_new(&camera->extras, rmodel));

  if (!strcmp(camera->type.c_str(), "perspective")) {
    rb_ivar_set(rcamera, rb_intern("@aspect_ratio"), DBL2NUM(camera->perspective.aspectRatio));
    rb_ivar_set(rcamera, rb_intern("@yfov"),         DBL2NUM(camera->perspective.yfov));
    rb_ivar_set(rcamera, rb_intern("@znear"),        DBL2NUM(camera->perspective.znear));
    if (camera->perspective.zfar == 0.0)
      rb_ivar_set(rcamera, rb_intern("@zfar"), rb_const_get(rb_cFloat, rb_intern("INFINITY")));
    else
      rb_ivar_set(rcamera, rb_intern("@zfar"), DBL2NUM(camera->perspective.zfar));
  } else {
    rb_ivar_set(rcamera, rb_intern("@xmag"),  DBL2NUM(camera->orthographic.xmag));
    rb_ivar_set(rcamera, rb_intern("@ymag"),  DBL2NUM(camera->orthographic.ymag));
    rb_ivar_set(rcamera, rb_intern("@znear"), DBL2NUM(camera->orthographic.znear));
    rb_ivar_set(rcamera, rb_intern("@zfar"),  DBL2NUM(camera->orthographic.zfar));
  }

  return rcamera;
}

VALUE rExtensionMap_new(const ExtensionMap *value, VALUE rmodel) {
  VALUE res = Qnil;

  for (ExtensionMap::const_iterator iterator = value->begin(); iterator != value->end(); iterator++) {
    if (NIL_P(res)) res = rb_hash_new();
    rb_hash_aset(res, rb_str_new2(iterator->first.c_str()), rValue_new(&iterator->second, rmodel));
  }

  return res;
}

void Image_set_image_data(const Image *img, VALUE rimg) {
  // Keep (yes duplicate but the original will be freed) the buffer data
  // in memory w/o having to wrap with a ruby string, which can be expensive
  // and inefficient.
  Image *dst = Image_unwrap(rimg);
  dst->image = img->image;
  dst->uri  = img->uri;

  rb_ivar_set(rimg, rb_intern("@width"),             INT2NUM(img->width));
  rb_ivar_set(rimg, rb_intern("@height"),            INT2NUM(img->height));
  rb_ivar_set(rimg, rb_intern("@components"),        INT2NUM(img->component));
}

VALUE rImage_new(const Image *img, VALUE rmodel) {
  VALUE rimg = rb_funcall(rb_cImage, rb_intern("new"), 0);

  Image_set_image_data(img, rimg);
  rb_ivar_set(rimg, rb_intern("@model"),             rmodel);
  rb_ivar_set(rimg, rb_intern("@name"),              rb_str_new2(img->name.c_str()));
  rb_ivar_set(rimg, rb_intern("@mime_type"),         Qnil);
  rb_ivar_set(rimg, rb_intern("@extras"),            rValue_new(&img->extras, rmodel));
  rb_ivar_set(rimg, rb_intern("@buffer_view_index"), RINDEX_OR_NIL(img->bufferView));
  rb_ivar_set(rimg, rb_intern("@extensions"),        rExtensionMap_new(&img->extensions, rmodel));

  if (img->mimeType.size() > 0)
    rb_ivar_set(rimg, rb_intern("@mime_type"), rb_str_new2(img->mimeType.c_str()));

  return rimg;
}

VALUE Image_set_uri(VALUE self, VALUE ruri) {
  if (NIL_P(ruri)) return self;
  const char *uri = StringValueCStr(ruri);
  std::string str = uri;
  if (IsDataURI(str)) {
    std::vector<unsigned char> data;
    std::string mime_type;
    if (DecodeDataURI(&data, mime_type, str, strlen(uri), false)) {
      std::string err, warn;
      Image img;
      rb_ivar_set(self, rb_intern("@mime_type"),  rb_str_new2(mime_type.c_str()));
      memset(&img, 0, sizeof(img));
      if (tinygltf::LoadImageData(&img, -1, &err, &warn, 0, 0, data.data(), (int) data.size(), NULL)) {
        Image_set_image_data(&img, self);
      } else {
        rb_raise(rb_eStandardError, "Could not parse image data: %s", err.c_str());
      }
      if (warn.size() > 0) {
        rb_warning("%s", warn.c_str());
      }
    } else {
      rb_raise(rb_eStandardError, "Could not decode data URI");
    }
  }
  return self;
}

VALUE Image_to_ptr(VALUE self) {
  // return Fiddle::Pointer.new(addr, size)
  VALUE rFiddlePointer = rb_const_get(rb_const_get(rb_cObject, rb_intern("Fiddle")),
                                      rb_intern("Pointer"));
  Image *img = Image_unwrap(self);
  return rb_funcall(rFiddlePointer, rb_intern("new"), 2,
                    ULL2NUM((unsigned long long) img->image.data()),
                    ULL2NUM((unsigned long long) img->image.size()));
}

VALUE Image_size(VALUE self) {
  return ULONG2NUM(Image_unwrap(self)->image.size());
}

VALUE Image_to_s(VALUE self) {
  Image *img = Image_unwrap(self);
  return rb_str_new((char *) img->image.data(), img->image.size());
}

VALUE Image_uri(VALUE self) {
  Image *img = Image_unwrap(self);
  if (img->uri.size() > 0)
    return rb_str_new2(img->uri.c_str());
  else
    return Qnil;
}

VALUE rLight_new(const Light *light, VALUE rmodel) {
  VALUE rlight = rb_funcall(rb_cLight, rb_intern("new"), 0);
  // *Light_unwrap(rlight) = *light;

  VALUE rcolor = rb_ary_new();
  for (size_t i = 0; i < light->color.size(); i++)
    rb_ary_push(rcolor, DBL2NUM(light->color[i]));

  rb_ivar_set(rlight, rb_intern("@model"), rmodel);
  rb_ivar_set(rlight, rb_intern("@name"),  rb_str_new2(light->name.c_str()));
  rb_ivar_set(rlight, rb_intern("@type"),  rb_intern(light->type.c_str()));
  rb_ivar_set(rlight, rb_intern("@color"), rcolor);

  return rlight;
}

VALUE rMaterial_new(const Material *matr, VALUE rmodel) {
  VALUE rmatr = rb_funcall(rb_cMaterial, rb_intern("new"), 0);
  // *Material_unwrap(rmatr) = *matr;

  rb_ivar_set(rmatr, rb_intern("@model"),             rmodel);
  rb_ivar_set(rmatr, rb_intern("@name"),              rb_str_new2(matr->name.c_str()));
  rb_ivar_set(rmatr, rb_intern("@values"),            rParameterMap_new(&matr->values, rmodel));
  rb_ivar_set(rmatr, rb_intern("@additional_values"), rParameterMap_new(&matr->additionalValues, rmodel));
  rb_ivar_set(rmatr, rb_intern("@extensions"),        rExtensionMap_new(&matr->extensions, rmodel));
  rb_ivar_set(rmatr, rb_intern("@extras"),            rValue_new(&matr->extras, rmodel));

  return rmatr;
}

VALUE rMesh_new(const Mesh *mesh, VALUE rmodel) {
  VALUE rmesh = rb_funcall(rb_cMesh, rb_intern("new"), 0);
  // *Mesh_unwrap(rmesh) = *mesh;

  VALUE rprimitives = rb_ary_new();
  for (size_t i = 0; i < mesh->primitives.size(); i++) {
    rb_ary_push(rprimitives, rPrimitive_new(&mesh->primitives[i], rmodel));
  }

  VALUE rweights = rb_ary_new();
  for (size_t i = 0; i < mesh->weights.size(); i++) {
    rb_ary_push(rweights, DBL2NUM(mesh->weights[i]));
  }

  rb_ivar_set(rmesh, rb_intern("@model"),          rmodel);
  rb_ivar_set(rmesh, rb_intern("@name"),           rb_str_new2(mesh->name.c_str()));
  rb_ivar_set(rmesh, rb_intern("@primitives"),     rprimitives);
  rb_ivar_set(rmesh, rb_intern("@weights"),        rweights);
  rb_ivar_set(rmesh, rb_intern("@extensions"),     rExtensionMap_new(&mesh->extensions, rmodel));
  rb_ivar_set(rmesh, rb_intern("@extras"),         rValue_new(&mesh->extras, rmodel));

  return rmesh;
}

VALUE rModel_new(const Model *model) {
  VALUE rmodel = rb_funcall(rb_cModel, rb_intern("new"), 0);
  // *Model_unwrap(rmodel) = *model;

  VALUE rext_used = rb_ary_new();
  for (size_t i = 0; i < model->extensionsUsed.size(); i++)
    rb_ary_push(rext_used, rb_str_new2(model->extensionsUsed[i].c_str()));

  VALUE rext_required = rb_ary_new();
  for (size_t i = 0; i < model->extensionsRequired.size(); i++)
    rb_ary_push(rext_required, rb_str_new2(model->extensionsRequired[i].c_str()));

  rb_ivar_set(rmodel, rb_intern("@asset"),               rAsset_new(&model->asset, rmodel));
  rb_ivar_set(rmodel, rb_intern("@default_scene_index"), RINDEX_OR_NIL(model->defaultScene));
  rb_ivar_set(rmodel, rb_intern("@extensions"),          rExtensionMap_new(&model->extensions, rmodel));
  rb_ivar_set(rmodel, rb_intern("@extras"),              rValue_new(&model->extras, rmodel));
  rb_ivar_set(rmodel, rb_intern("@extensions_used"),     rext_used);
  rb_ivar_set(rmodel, rb_intern("@extensions_required"), rext_required);

  /*
    VALUE ary = rb_funcall(rmodel, rb_intern("accessors"), 0);
    for (size_t i = 0; i < model->accessors.size(); i++) {
      rb_ary_push(ary, rAccessor_new(&model->accessors[i], rmodel));
    }
  */
  #define CONCAT_VECTOR_TO_RARRAY3(klass, name, method) {            \
    VALUE ary = rb_funcall(rmodel, rb_intern(method), 0);            \
    for (size_t i = 0; i < model->name.size(); i++) {                \
      rb_ary_push(ary, r ## klass ## _new(&model->name[i], rmodel)); \
    }                                                                \
  }
  #define CONCAT_VECTOR_TO_RARRAY(klass, name) CONCAT_VECTOR_TO_RARRAY3(klass, name, #name)

  CONCAT_VECTOR_TO_RARRAY(Accessor,    accessors);
  CONCAT_VECTOR_TO_RARRAY(Animation,   animations);
  CONCAT_VECTOR_TO_RARRAY(Buffer,      buffers);
  CONCAT_VECTOR_TO_RARRAY3(BufferView, bufferViews, "buffer_views");
  CONCAT_VECTOR_TO_RARRAY(Material,    materials);
  CONCAT_VECTOR_TO_RARRAY(Mesh,        meshes);
  CONCAT_VECTOR_TO_RARRAY(Node,        nodes);
  CONCAT_VECTOR_TO_RARRAY(Texture,     textures);
  CONCAT_VECTOR_TO_RARRAY(Image,       images);
  CONCAT_VECTOR_TO_RARRAY(Skin,        skins);
  CONCAT_VECTOR_TO_RARRAY(Sampler,     samplers);
  CONCAT_VECTOR_TO_RARRAY(Camera,      cameras);
  CONCAT_VECTOR_TO_RARRAY(Scene,       scenes);
  CONCAT_VECTOR_TO_RARRAY(Light,       lights);

  return rmodel;
}

VALUE rNode_new(const Node *node, VALUE rmodel) {
  VALUE rnode = rb_funcall(rb_cNode, rb_intern("new"), 0);
  // *Node_unwrap(rnode) = *node;

  VALUE rweights = rb_ary_new();
  for (size_t i = 0; i < node->weights.size(); i++)
    rb_ary_push(rweights, DBL2NUM(node->weights[i]));

  VALUE rchildren = rb_ary_new();
  for (size_t i = 0; i < node->children.size(); i++)
    rb_ary_push(rchildren, RINDEX_OR_NIL(node->children[i]));

  VALUE rmatrix = Qnil, rrotation = Qnil, rtranslation = Qnil, rscale = Qnil;
  if (node->matrix.size() == 0) {
    rtranslation = rb_ary_new();
    if (node->translation.size() == 0) {
      rb_ary_push(rtranslation, DBL2NUM(0.0));
      rb_ary_push(rtranslation, DBL2NUM(0.0));
      rb_ary_push(rtranslation, DBL2NUM(0.0));
    } else {
      for (size_t i = 0; i < node->translation.size(); i++)
        rb_ary_push(rtranslation, DBL2NUM(node->translation[i]));
    }

    rrotation = rb_ary_new();
    if (node->rotation.size() == 0) {
      rb_ary_push(rrotation, DBL2NUM(0.0));
      rb_ary_push(rrotation, DBL2NUM(0.0));
      rb_ary_push(rrotation, DBL2NUM(0.0));
      rb_ary_push(rrotation, DBL2NUM(1.0));
    } else {
      for (size_t i = 0; i < node->rotation.size(); i++)
        rb_ary_push(rrotation, DBL2NUM(node->rotation[i]));
    }

    rscale = rb_ary_new();
    if (node->scale.size() == 0) {
      rb_ary_push(rscale, DBL2NUM(1.0));
      rb_ary_push(rscale, DBL2NUM(1.0));
      rb_ary_push(rscale, DBL2NUM(1.0));
    } else {
      for (size_t i = 0; i < node->scale.size(); i++)
        rb_ary_push(rscale, DBL2NUM(node->scale[i]));
    }
  } else {
    rmatrix = rb_ary_new();
    for (size_t i = 0; i < node->matrix.size(); i++)
      rb_ary_push(rmatrix, DBL2NUM(node->matrix[i]));
  }

  rb_ivar_set(rnode, rb_intern("@model"),            rmodel);
  rb_ivar_set(rnode, rb_intern("@name"),             rb_str_new2(node->name.c_str()));
  rb_ivar_set(rnode, rb_intern("@camera_index"),     RINDEX_OR_NIL(node->camera));
  rb_ivar_set(rnode, rb_intern("@skin_index"),       RINDEX_OR_NIL(node->skin));
  rb_ivar_set(rnode, rb_intern("@mesh_index"),       RINDEX_OR_NIL(node->mesh));
  rb_ivar_set(rnode, rb_intern("@children_indices"), rchildren);
  rb_ivar_set(rnode, rb_intern("@rotation"),         rrotation);
  rb_ivar_set(rnode, rb_intern("@translation"),      rtranslation);
  rb_ivar_set(rnode, rb_intern("@scale"),            rscale);
  rb_ivar_set(rnode, rb_intern("@matrix"),           rmatrix);
  rb_ivar_set(rnode, rb_intern("@weights"),          rweights);
  rb_ivar_set(rnode, rb_intern("@extensions"),       rExtensionMap_new(&node->extensions, rmodel));
  rb_ivar_set(rnode, rb_intern("@extras"),           rValue_new(&node->extras, rmodel));

  return rnode;
}

VALUE rParameterMap_new(const ParameterMap *value, VALUE rmodel) {
  VALUE res = Qnil;

  for (ParameterMap::const_iterator paramIt = value->begin(); paramIt != value->end();
       ++paramIt) {
    if (NIL_P(res)) res = rb_hash_new();
    VALUE key = ID2SYM(rb_intern(paramIt->first.c_str()));
    VALUE val = Qnil;

    if (paramIt->second.number_array.size()) {
      val = rb_ary_new();
      for (size_t i = 0; i < paramIt->second.number_array.size(); i++)
        rb_ary_push(val, DBL2NUM(paramIt->second.number_array[i]));
    } else if (paramIt->second.json_double_value.size()) {
      val = rb_hash_new();
      for (std::map<std::string, double>::const_iterator it =
               paramIt->second.json_double_value.begin();
           it != paramIt->second.json_double_value.end(); ++it) {
        if (it->first == "index") {
          rb_hash_aset(val, ID2SYM(rb_intern("index")), RINDEX_OR_NIL(paramIt->second.TextureIndex()));
        } else {
          rb_hash_aset(val, ID2SYM(rb_intern(it->first.c_str())), DBL2NUM(it->second));
        }
      }
    } else if (!paramIt->second.string_value.empty()) {
      val = rb_str_new2(paramIt->second.string_value.c_str());
    } else if (paramIt->second.has_number_value) {
      val = DBL2NUM(paramIt->second.number_value);
    } else {
      val = paramIt->second.bool_value ? Qtrue : Qfalse;
    }

    rb_hash_aset(res, key, val);
  }

  return res;
}

VALUE rPrimitive_new(const Primitive *prim, VALUE rmodel) {
  VALUE rprim = rb_funcall(rb_cPrimitive, rb_intern("new"), 0);
  // *Primitive_unwrap(rprim) = *prim;

  VALUE rattrs = rb_hash_new();
  for (std::map<std::string, int>::const_iterator it = prim->attributes.begin();
       it != prim->attributes.end(); ++it) {
    std::string key = it->first;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    rb_hash_aset(rattrs, ID2SYM(rb_intern(key.c_str())), INT2NUM(it->second));
  }

  VALUE rtargets = rb_ary_new();
  for (size_t i = 0; i < prim->targets.size(); i++) {
    VALUE rtarget = rb_hash_new();
    rb_ary_push(rtargets, rtarget);
    for (std::map<std::string, int>::const_iterator it = prim->targets[i].begin();
         it != prim->targets[i].end(); ++it) {
      std::string key = it->first;
      std::transform(key.begin(), key.end(), key.begin(), ::tolower);
      rb_hash_aset(rtarget, ID2SYM(rb_intern(key.c_str())), INT2NUM(it->second));
    }
  }

  rb_ivar_set(rprim, rb_intern("@model"),                 rmodel);
  rb_ivar_set(rprim, rb_intern("@attributes"),            rattrs);
  rb_ivar_set(rprim, rb_intern("@material_index"),        RINDEX_OR_NIL(prim->material));
  rb_ivar_set(rprim, rb_intern("@indices_index"),         RINDEX_OR_NIL(prim->indices));
  rb_ivar_set(rprim, rb_intern("@mode"),                  mode_to_sym(prim->mode));
  rb_ivar_set(rprim, rb_intern("@morph_targets_indices"), rtargets);
  rb_ivar_set(rprim, rb_intern("@extras"),                rValue_new(&prim->extras, rmodel));

  return rprim;
}

VALUE rSampler_new(const Sampler *sampler, VALUE rmodel) {
  VALUE rsampler = rb_funcall(rb_cSampler, rb_intern("new"), 0);
  // *Sampler_unwrap(rsampler) = *sampler;

  rb_ivar_set(rsampler, rb_intern("@model"),      rmodel);
  rb_ivar_set(rsampler, rb_intern("@name"),       rb_str_new2(sampler->name.c_str()));
  rb_ivar_set(rsampler, rb_intern("@min_filter"), texture_filter_to_sym(sampler->minFilter));
  rb_ivar_set(rsampler, rb_intern("@mag_filter"), texture_filter_to_sym(sampler->magFilter));
  rb_ivar_set(rsampler, rb_intern("@wrap_s"),     texture_wrap_to_sym(sampler->wrapS));
  rb_ivar_set(rsampler, rb_intern("@wrap_t"),     texture_wrap_to_sym(sampler->wrapT));
  // removed from tiny_gltf until a spec or extension actually calls for it
  // https://github.com/syoyo/tinygltf/issues/287
  // rb_ivar_set(rsampler, rb_intern("@wrap_r"),     texture_wrap_to_sym(sampler->wrapR));
  rb_ivar_set(rsampler, rb_intern("@extras"),     rValue_new(&sampler->extras, rmodel));

  return rsampler;
}

VALUE rScene_new(const Scene *scene, VALUE rmodel) {
  VALUE rscene = rb_funcall(rb_cScene, rb_intern("new"), 0);
  // *Scene_unwrap(rscene) = *scene;

  VALUE rnodes = rb_ary_new();
  for (size_t i = 0; i < scene->nodes.size(); i++)
    rb_ary_push(rnodes, RINDEX_OR_NIL(scene->nodes[i]));

  rb_ivar_set(rscene, rb_intern("@model"),         rmodel);
  rb_ivar_set(rscene, rb_intern("@name"),          rb_str_new2(scene->name.c_str()));
  rb_ivar_set(rscene, rb_intern("@nodes_indices"), rnodes);
  rb_ivar_set(rscene, rb_intern("@extensions"),    rExtensionMap_new(&scene->extensions, rmodel));
  rb_ivar_set(rscene, rb_intern("@extras"),        rValue_new(&scene->extras, rmodel));

  return rscene;
}

VALUE rSkin_new(const Skin *skin, VALUE rmodel) {
  VALUE rskin = rb_funcall(rb_cSkin, rb_intern("new"), 0);
  // *Skin_unwrap(rskin) = *skin;

  VALUE rjoints = rb_ary_new();
  for (size_t i = 0; i < skin->joints.size(); i++)
    rb_ary_push(rjoints, RINDEX_OR_NIL(skin->joints[i]));

  rb_ivar_set(rskin, rb_intern("@model"),                    rmodel);
  rb_ivar_set(rskin, rb_intern("@name"),                     rb_str_new2(skin->name.c_str()));
  rb_ivar_set(rskin, rb_intern("@inverse_bind_matrices"),    RINDEX_OR_NIL(skin->inverseBindMatrices));
  rb_ivar_set(rskin, rb_intern("@skeleton_root_node_index"), RINDEX_OR_NIL(skin->skeleton));
  rb_ivar_set(rskin, rb_intern("@joint_nodes_indices"),      rjoints);

  return rskin;
}

VALUE rTexture_new(const Texture *texture, VALUE rmodel) {
  VALUE rtexture = rb_funcall(rb_cTexture, rb_intern("new"), 0);
  // *Texture_unwrap(rtexture) = *texture;

  rb_ivar_set(rtexture, rb_intern("@model"),         rmodel);
  rb_ivar_set(rtexture, rb_intern("@name"),          rb_str_new2(texture->name.c_str()));
  rb_ivar_set(rtexture, rb_intern("@sampler_index"), RINDEX_OR_NIL(texture->sampler));
  rb_ivar_set(rtexture, rb_intern("@source_index"),  RINDEX_OR_NIL(texture->source));
  rb_ivar_set(rtexture, rb_intern("@extensions"),    rExtensionMap_new(&texture->extensions, rmodel));
  rb_ivar_set(rtexture, rb_intern("@extras"),        rValue_new(&texture->extras, rmodel));

  return rtexture;
}

/*
  void Model_free(void* data) {
    Model *obj = (Model *) data;
    delete obj;
  }

  size_t Model_size(const void* data) {
    return sizeof(Model);
  }

  static const rb_data_type_t T_Model = {
    .wrap_struct_name = "TinyGLTFModel",
    .function = {
      .dmark = NULL,
      .dfree = Model_free,
      .dsize = Model_size
    },
    .parent = NULL,
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY
  };

  VALUE Model_alloc(VALUE klass) {
    return TypedData_Wrap_Struct(klass, &T_Model, new Model());
  }

  VALUE Model_is_equal(VALUE self, VALUE other) {
    Model *a, *b;
    TypedData_Get_Struct(self,  Model, &T_Model, a);
    TypedData_Get_Struct(other, Model, &T_Model, b);
    return (*a) == (*b) ? Qtrue : Qfalse;
  }
*/
#define DEFN_GLTF_TYPE(klass)                                       \
  void klass ## _free(void* data) {                                 \
    klass *obj = (klass *) data;                                    \
    delete obj;                                                     \
  }                                                                 \
                                                                    \
  size_t klass ## _size(const void* data) {                         \
    return sizeof(klass);                                           \
  }                                                                 \
                                                                    \
  const rb_data_type_t T_ ## klass = {                              \
    .wrap_struct_name = "TinyGLTF" #klass,                          \
    .function = {                                                   \
      .dmark = NULL,                                                \
      .dfree = klass ## _free,                                      \
      .dsize = klass ## _size                                       \
    },                                                              \
    .parent = NULL,                                                 \
    .data = NULL,                                                   \
    .flags = RUBY_TYPED_FREE_IMMEDIATELY                            \
  };                                                                \
                                                                    \
  VALUE klass ## _alloc(VALUE k) {                                  \
    return TypedData_Wrap_Struct(k, &T_ ## klass, new klass());     \
  }                                                                 \
                                                                    \
  VALUE klass ## _is_equal(VALUE self, VALUE other) {               \
    klass *a, *b;                                                   \
    TypedData_Get_Struct(self,  klass, &T_ ## klass, a);            \
    TypedData_Get_Struct(other, klass, &T_ ## klass, b);            \
    return (*a) == (*b) ? Qtrue : Qfalse;                           \
  }

DEFN_GLTF_TYPE(Model);
DEFN_GLTF_TYPE(Asset);
DEFN_GLTF_TYPE(Accessor);
DEFN_GLTF_TYPE(Animation);
DEFN_GLTF_TYPE(AnimationChannel);
DEFN_GLTF_TYPE(AnimationSampler);
DEFN_GLTF_TYPE(Buffer);
DEFN_GLTF_TYPE(BufferView);
DEFN_GLTF_TYPE(Material);
DEFN_GLTF_TYPE(Mesh);
DEFN_GLTF_TYPE(Node);
DEFN_GLTF_TYPE(Primitive);
DEFN_GLTF_TYPE(Texture);
DEFN_GLTF_TYPE(Image);
DEFN_GLTF_TYPE(Skin);
DEFN_GLTF_TYPE(Sampler);
DEFN_GLTF_TYPE(Camera);
DEFN_GLTF_TYPE(Scene);
DEFN_GLTF_TYPE(Light);

VALUE mode_to_sym(int mode) {
  switch(mode) {
    case TINYGLTF_MODE_POINTS:         return ID2SYM(rb_intern("points"));
    case TINYGLTF_MODE_LINE:           return ID2SYM(rb_intern("line"));
    case TINYGLTF_MODE_LINE_LOOP:      return ID2SYM(rb_intern("line_loop"));
    case TINYGLTF_MODE_TRIANGLES:      return ID2SYM(rb_intern("triangles"));
    case TINYGLTF_MODE_TRIANGLE_STRIP: return ID2SYM(rb_intern("triangle_strip"));
    case TINYGLTF_MODE_TRIANGLE_FAN:   return ID2SYM(rb_intern("triangle_fan"));
    default:                           return ID2SYM(rb_intern("unknown"));
  }
}

VALUE component_type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_COMPONENT_TYPE_BYTE:           return ID2SYM(rb_intern("byte"));
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:  return ID2SYM(rb_intern("ubyte"));
    case TINYGLTF_COMPONENT_TYPE_SHORT:          return ID2SYM(rb_intern("short"));
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: return ID2SYM(rb_intern("ushort"));
    case TINYGLTF_COMPONENT_TYPE_INT:            return ID2SYM(rb_intern("int"));
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:   return ID2SYM(rb_intern("uint"));
    case TINYGLTF_COMPONENT_TYPE_FLOAT:          return ID2SYM(rb_intern("float"));
    case TINYGLTF_COMPONENT_TYPE_DOUBLE:         return ID2SYM(rb_intern("double"));
    default:                                     return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_filter_to_sym(int filter) {
  switch(filter) {
    case TINYGLTF_TEXTURE_FILTER_NEAREST:                return ID2SYM(rb_intern("nearest"));
    case TINYGLTF_TEXTURE_FILTER_LINEAR:                 return ID2SYM(rb_intern("linear"));
    case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST: return ID2SYM(rb_intern("nearest_mipmap_nearest"));
    case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:  return ID2SYM(rb_intern("linear_mipmap_nearest"));
    case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:  return ID2SYM(rb_intern("nearest_mipmap_linear"));
    case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:   return ID2SYM(rb_intern("linear_mipmap_linear"));
    default:                                             return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_wrap_to_sym(int wrap) {
  switch(wrap) {
    case TINYGLTF_TEXTURE_WRAP_REPEAT:          return ID2SYM(rb_intern("repeat"));
    case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:   return ID2SYM(rb_intern("clamp_to_edge"));
    case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT: return ID2SYM(rb_intern("mirrored_repeat"));
    default:                                    return ID2SYM(rb_intern("unknown"));
  }
}

VALUE parameter_type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_PARAMETER_TYPE_BYTE:           return ID2SYM(rb_intern("byte"));
    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:  return ID2SYM(rb_intern("ubyte"));
    case TINYGLTF_PARAMETER_TYPE_SHORT:          return ID2SYM(rb_intern("short"));
    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: return ID2SYM(rb_intern("ushort"));
    case TINYGLTF_PARAMETER_TYPE_INT:            return ID2SYM(rb_intern("int"));
    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:   return ID2SYM(rb_intern("uint"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT:          return ID2SYM(rb_intern("float"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_VEC2:     return ID2SYM(rb_intern("float_vec2"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_VEC3:     return ID2SYM(rb_intern("float_vec3"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_VEC4:     return ID2SYM(rb_intern("float_vec4"));
    case TINYGLTF_PARAMETER_TYPE_INT_VEC2:       return ID2SYM(rb_intern("int_vec2"));
    case TINYGLTF_PARAMETER_TYPE_INT_VEC3:       return ID2SYM(rb_intern("int_vec3"));
    case TINYGLTF_PARAMETER_TYPE_INT_VEC4:       return ID2SYM(rb_intern("int_vec4"));
    case TINYGLTF_PARAMETER_TYPE_BOOL:           return ID2SYM(rb_intern("bool"));
    case TINYGLTF_PARAMETER_TYPE_BOOL_VEC2:      return ID2SYM(rb_intern("bool_vec2"));
    case TINYGLTF_PARAMETER_TYPE_BOOL_VEC3:      return ID2SYM(rb_intern("bool_vec3"));
    case TINYGLTF_PARAMETER_TYPE_BOOL_VEC4:      return ID2SYM(rb_intern("bool_vec4"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_MAT2:     return ID2SYM(rb_intern("float_mat2"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_MAT3:     return ID2SYM(rb_intern("float_mat3"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_MAT4:     return ID2SYM(rb_intern("float_mat4"));
    case TINYGLTF_PARAMETER_TYPE_SAMPLER_2D:     return ID2SYM(rb_intern("sampler_2d"));
    default:                                     return ID2SYM(rb_intern("unknown"));
  }
}

VALUE type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_TYPE_VEC2:   return ID2SYM(rb_intern("vec2"));
    case TINYGLTF_TYPE_VEC3:   return ID2SYM(rb_intern("vec3"));
    case TINYGLTF_TYPE_VEC4:   return ID2SYM(rb_intern("vec4"));
    case TINYGLTF_TYPE_MAT2:   return ID2SYM(rb_intern("mat2"));
    case TINYGLTF_TYPE_MAT3:   return ID2SYM(rb_intern("mat3"));
    case TINYGLTF_TYPE_MAT4:   return ID2SYM(rb_intern("mat4"));
    case TINYGLTF_TYPE_SCALAR: return ID2SYM(rb_intern("scalar"));
    case TINYGLTF_TYPE_VECTOR: return ID2SYM(rb_intern("vector"));
    case TINYGLTF_TYPE_MATRIX: return ID2SYM(rb_intern("matrix"));
    default:                   return ID2SYM(rb_intern("unknown"));
  }
}

VALUE image_format_to_sym(int fmt) {
  switch(fmt) {
    case TINYGLTF_IMAGE_FORMAT_JPEG: return ID2SYM(rb_intern("jpeg"));
    case TINYGLTF_IMAGE_FORMAT_PNG:  return ID2SYM(rb_intern("png"));
    case TINYGLTF_IMAGE_FORMAT_BMP:  return ID2SYM(rb_intern("bmp"));
    case TINYGLTF_IMAGE_FORMAT_GIF:  return ID2SYM(rb_intern("gif"));
    default:                         return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_format_to_sym(int fmt) {
  switch(fmt) {
    case TINYGLTF_TEXTURE_FORMAT_ALPHA:           return ID2SYM(rb_intern("alpha"));
    case TINYGLTF_TEXTURE_FORMAT_RGB:             return ID2SYM(rb_intern("rgb"));
    case TINYGLTF_TEXTURE_FORMAT_RGBA:            return ID2SYM(rb_intern("rgba"));
    case TINYGLTF_TEXTURE_FORMAT_LUMINANCE:       return ID2SYM(rb_intern("luminance"));
    case TINYGLTF_TEXTURE_FORMAT_LUMINANCE_ALPHA: return ID2SYM(rb_intern("luminance_alpha"));
    default:                                      return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_target_to_sym(int tgt) {
  switch(tgt) {
    case TINYGLTF_TEXTURE_TARGET_TEXTURE2D: return ID2SYM(rb_intern("texture2d"));
    default:                                return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_TEXTURE_TYPE_UNSIGNED_BYTE: return ID2SYM(rb_intern("ubyte"));
    default:                                  return ID2SYM(rb_intern("unknown"));
  }
}

VALUE target_to_sym(int tgt) {
  switch(tgt) {
    case TINYGLTF_TARGET_ARRAY_BUFFER:         return ID2SYM(rb_intern("array_buffer"));
    case TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER: return ID2SYM(rb_intern("element_array_buffer"));
    default:                                   return ID2SYM(rb_intern("unknown"));
  }
}

VALUE shader_type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_SHADER_TYPE_VERTEX_SHADER:   return ID2SYM(rb_intern("vertex_shader"));
    case TINYGLTF_SHADER_TYPE_FRAGMENT_SHADER: return ID2SYM(rb_intern("fragment_shader"));
    default:                                   return ID2SYM(rb_intern("unknown"));
  }
}

VALUE rValue_new(const Value *value, VALUE rmodel) {
  switch(value->Type()) {
    case NULL_TYPE:   return Qnil;
    case REAL_TYPE:   return DBL2NUM(value->Get<double>());
    case INT_TYPE:    return INT2NUM(value->Get<int>());
    case BOOL_TYPE:   return value->Get<bool>() ? Qtrue : Qfalse;
    case STRING_TYPE: return rb_str_new2(value->Get<std::string>().c_str());
    case BINARY_TYPE: {
      std::vector<unsigned char> val = value->Get<std::vector<unsigned char>>();
      return rb_str_new((char *) val.data(), val.size());
    }
    case ARRAY_TYPE: {
      VALUE ary = rb_ary_new();
      for (size_t i = 0; i < value->ArrayLen(); i++) {
        rb_ary_push(ary, rValue_new(&value->Get((int) i), rmodel));
      }
      return ary;
    }
    case OBJECT_TYPE: {
      VALUE hash = rb_hash_new();
      std::vector<std::string> keys = value->Keys();
      for (std::string key : keys) {
        rb_hash_aset(hash, rb_str_new2(key.c_str()), rValue_new(&value->Get(key), rmodel));
      }
      return hash;
    }
    default:
      rb_raise(rb_eRuntimeError, "Don't know what to do with GLTF type %d", (int) value->Type());
      return Qnil;
  }
}

void Init_tiny_gltf(void) {
  rb_mTinyGLTF = rb_define_module("TinyGLTF");

  // not part of the macro below just to help the yardoc parser out
  rb_cModel            = rb_define_class_under(rb_mTinyGLTF, "Model",            rb_cObject);
  rb_cAccessor         = rb_define_class_under(rb_mTinyGLTF, "Accessor",         rb_cObject);
  rb_cAsset            = rb_define_class_under(rb_mTinyGLTF, "Asset",            rb_cObject);
  rb_cAnimation        = rb_define_class_under(rb_mTinyGLTF, "Animation",        rb_cObject);
  rb_cAnimationChannel = rb_define_class_under(rb_mTinyGLTF, "AnimationChannel", rb_cObject);
  rb_cAnimationSampler = rb_define_class_under(rb_mTinyGLTF, "AnimationSampler", rb_cObject);
  rb_cBuffer           = rb_define_class_under(rb_mTinyGLTF, "Buffer",           rb_cObject);
  rb_cBufferView       = rb_define_class_under(rb_mTinyGLTF, "BufferView",       rb_cObject);
  rb_cMaterial         = rb_define_class_under(rb_mTinyGLTF, "Material",         rb_cObject);
  rb_cMesh             = rb_define_class_under(rb_mTinyGLTF, "Mesh",             rb_cObject);
  rb_cNode             = rb_define_class_under(rb_mTinyGLTF, "Node",             rb_cObject);
  rb_cTexture          = rb_define_class_under(rb_mTinyGLTF, "Texture",          rb_cObject);
  rb_cImage            = rb_define_class_under(rb_mTinyGLTF, "Image",            rb_cObject);
  rb_cPrimitive        = rb_define_class_under(rb_mTinyGLTF, "Primitive",        rb_cObject);
  rb_cSkin             = rb_define_class_under(rb_mTinyGLTF, "Skin",             rb_cObject);
  rb_cSampler          = rb_define_class_under(rb_mTinyGLTF, "Sampler",          rb_cObject);
  rb_cCamera           = rb_define_class_under(rb_mTinyGLTF, "Camera",           rb_cObject);
  rb_cScene            = rb_define_class_under(rb_mTinyGLTF, "Scene",            rb_cObject);
  rb_cLight            = rb_define_class_under(rb_mTinyGLTF, "Light",            rb_cObject);

  /*
    rb_define_alloc_func(rb_cModel, Model_alloc);
    rb_define_method(rb_cModel, "==", Model_is_equal, 1);
  */
  #define DEFINE_RB_GLTF_CLASS(name)                                         \
    rb_define_alloc_func(rb_c ## name, name ## _alloc);                      \
    rb_define_method(rb_c ## name, "==", name ## _is_equal, 1);

  DEFINE_RB_GLTF_CLASS(Model);
  DEFINE_RB_GLTF_CLASS(Accessor);
  DEFINE_RB_GLTF_CLASS(Asset);
  DEFINE_RB_GLTF_CLASS(Animation);
  DEFINE_RB_GLTF_CLASS(AnimationChannel);
  DEFINE_RB_GLTF_CLASS(AnimationSampler);
  DEFINE_RB_GLTF_CLASS(Buffer);
  DEFINE_RB_GLTF_CLASS(BufferView);
  DEFINE_RB_GLTF_CLASS(Material);
  DEFINE_RB_GLTF_CLASS(Mesh);
  DEFINE_RB_GLTF_CLASS(Node);
  DEFINE_RB_GLTF_CLASS(Texture);
  DEFINE_RB_GLTF_CLASS(Image);
  DEFINE_RB_GLTF_CLASS(Primitive);
  DEFINE_RB_GLTF_CLASS(Skin);
  DEFINE_RB_GLTF_CLASS(Sampler);
  DEFINE_RB_GLTF_CLASS(Camera);
  DEFINE_RB_GLTF_CLASS(Scene);
  DEFINE_RB_GLTF_CLASS(Light);

  /*
   * Document-method: to_ptr
   *
   * Returns a Fiddle::Pointer representing the start of the buffer data in
   * memory. For performance, try to use this rather than #to_s.
   */
  rb_define_method(rb_cBuffer, "to_ptr", Buffer_to_ptr, 0);

  /*
   * Document-method: size
   *
   * Returns the size of the buffer data in bytes.
   */
  rb_define_method(rb_cBuffer, "size",   Buffer_size,   0);

  /*
   * Document-method: to_s
   *
   * Returns a string containing the raw buffer data. For better performance,
   * prefer #to_ptr where possible.
   */
  rb_define_method(rb_cBuffer, "to_s",   Buffer_to_s,   0);

  /*
   * Document-method: uri
   *
   * Returns the URI from which this data was retrieved, if available. Returns
   * `nil` if a URI was not used.
   */
  rb_define_method(rb_cBuffer, "uri",    Buffer_uri,    0);

  /*
   * Document-method: uri=
   */
  rb_define_method(rb_cImage,  "uri=",   Image_set_uri, 1);

  /*
   * Document-method: to_ptr
   *
   * Returns a Fiddle::Pointer representing the start of the image data in
   * memory. For performance, try to use this rather than #to_s.
   */
  rb_define_method(rb_cImage,  "to_ptr", Image_to_ptr,  0);

  /*
   * Document-method: size
   *
   * Returns the size of the image data in bytes.
   */
  rb_define_method(rb_cImage,  "size",   Image_size,    0);

  /*
   * Document-method: to_s
   *
   * Returns a string containing the raw image data. For better performance,
   * prefer #to_ptr where possible.
   */
  rb_define_method(rb_cImage,  "to_s",   Image_to_s,    0);

  /*
   * Document-method: uri
   *
   * Returns the URI from which this data was retrieved, if available. Returns
   * `nil` if a URI was not used.
   */
  rb_define_method(rb_cImage,  "uri",    Image_uri,     0);

  rb_eTinyGLTFError = rb_define_class_under(rb_mTinyGLTF, "Error", rb_eStandardError);
  rb_define_singleton_method(rb_mTinyGLTF, "load", rb_tgltf_load, -1);
}
