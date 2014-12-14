
#include "mod.h"

#include "RConstants.h"
#include "RSim.h"
#include "RPrepLayer.h"

RCPP_MODULE(snnMod) {
    Rcpp::class_<RSim>("RSim")
    .constructor<RConstants*,unsigned char, size_t>()
    .method("print", &RSim::print, "Print sim")
    .method("printLayers", &RSim::printLayers, "Print detailed information about layers")
    .method("setInputSpikes", &RSim::setInputSpikes, "Set input spikes")
    .method("printConn", &RSim::printConn, "Print sim connection map")
    .method("printInputSpikes", &RSim::printInputSpikes, "Print input spikes queue")
    .method("run", &RSim::run, "Run sim")
    .method("getLayerStat", &RSim::getLayerStat, "Get layer stat")
    .method("W", &RSim::W, "Get List of W matrices")
    .method("getStatLevel", &RSim::getStatLevel, "Get stat levels of layers")
    .method("setStatLevel", &RSim::setStatLevel, "Set stat levels of layers")
    ;
    Rcpp::class_<RConstants>("RConstants")
    .constructor<std::string>()
    .field("const_filename", &RConstants::const_filename, "Constants filename")
    .method("setValue", &RConstants::setValue, "Set value of constant with section and name")
    .method("print", &RConstants::print, "Print constants")
    ;
    Rcpp::class_<RPrepLayer>("RPrepLayer")
    .constructor<RConstants*,unsigned char, size_t>()
    .method("run", &RPrepLayer::run, "Run time series through preparation layer")
    .method("getList", &RPrepLayer::getList, "Get an answer of population in list")
    .method("getBinMatrix", &RPrepLayer::getBinMatrix, "Get an answer of population in binary matrix")
    .method("getResponse", &RPrepLayer::getResponse, "Run prep layer thourgh one x value for certain time_limit")
    .method("getStat", &RPrepLayer::getStat, "Get statistics")
    .method("getParams", &RPrepLayer::getParams, "Get params of tuning curves")
    ;
}
