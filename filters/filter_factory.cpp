#include "filter_factory.h"
#include "crop.h"
#include "grayscale.h"
#include "negative.h"
#include "sharpening.h"
#include "edge_detection.h"
#include "gaussian_blur.h"
#include "glass_distortion.h"

#include <stdexcept>
#include <string>

static void RequireParams(const FilterRequest& req, size_t expected) {
    if (req.params.size() != expected) {
        throw std::invalid_argument("Filter '" + req.name + "' expects " + std::to_string(expected) +
                                    " parameter(s), got " + std::to_string(req.params.size()));
    }
}

static double ParseDouble(const std::string& s, const std::string& filter_name) {
    size_t pos = 0;
    double val = 0.0;
    try {
        val = std::stod(s, &pos);
    } catch (...) {
        throw std::invalid_argument("Filter '" + filter_name + "': invalid numeric parameter '" + s + "'");
    }
    if (pos != s.size()) {
        throw std::invalid_argument("Filter '" + filter_name + "': invalid numeric parameter '" + s + "'");
    }
    return val;
}

static size_t ParsePositiveSizeT(const std::string& s, const std::string& filter_name) {
    size_t pos = 0;
    int64_t val = 0;
    try {
        val = std::stoll(s, &pos);
    } catch (...) {
        throw std::invalid_argument("Filter '" + filter_name + "': expected positive integer, got '" + s + "'");
    }
    if (pos != s.size() || val <= 0) {
        throw std::invalid_argument("Filter '" + filter_name + "': expected positive integer, got '" + s + "'");
    }
    return val;
}

std::unique_ptr<Filter> FilterFactory::Create(const FilterRequest& request) {
    const std::string& name = request.name;

    if (name == "crop") {
        RequireParams(request, 2);
        size_t w = ParsePositiveSizeT(request.params[0], name);
        size_t h = ParsePositiveSizeT(request.params[1], name);
        return std::make_unique<Crop>(w, h);
    }

    if (name == "gs") {
        RequireParams(request, 0);
        return std::make_unique<Grayscale>();
    }

    if (name == "neg") {
        RequireParams(request, 0);
        return std::make_unique<Negative>();
    }

    if (name == "sharp") {
        RequireParams(request, 0);
        return std::make_unique<Sharpening>();
    }

    if (name == "edge") {
        RequireParams(request, 1);
        double threshold = ParseDouble(request.params[0], name);
        if (threshold < 0.0 || threshold > 1.0) {
            throw std::invalid_argument("Filter 'edge': threshold must be in [0.0, 1.0]");
        }
        return std::make_unique<EdgeDetection>(threshold);
    }

    if (name == "blur") {
        RequireParams(request, 1);
        double sigma = ParseDouble(request.params[0], name);
        if (sigma <= 0.0) {
            throw std::invalid_argument("Filter 'blur': sigma must be positive");
        }
        return std::make_unique<GaussianBlur>(sigma);
    }

    if (name == "glass") {
        RequireParams(request, 1);
        double scale = ParseDouble(request.params[0], name);
        if (scale <= 0.0) {
            throw std::invalid_argument("Filter 'glass': scale must be positive");
        }
        return std::make_unique<GlassDistortion>(scale);
    }

    throw std::invalid_argument("Unknown filter: '" + name + "'");
}