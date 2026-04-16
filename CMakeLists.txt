add_executable(
        image_processor
        image_processor.cpp

        image/image.cpp

        bmp/bmp_reader.cpp
        bmp/bmp_writer.cpp

        parser/parser.cpp

        pipeline/pipeline.cpp

        filters/matrix_filter.cpp
        filters/grayscale.cpp
        filters/negative.cpp
        filters/sharpening.cpp
        filters/edge_detection.cpp
        filters/crop.cpp
        filters/gaussian_blur.cpp
        filters/glass_distortion.cpp
        filters/filter_factory.cpp
)
