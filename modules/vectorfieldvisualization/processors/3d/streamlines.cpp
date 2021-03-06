#include "streamlines.h"

#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <modules/opengl/image/layergl.h>
#include <bitset>

#include <inviwo/core/util/imagesampler.h>

namespace inviwo {

const ProcessorInfo StreamLines::processorInfo_{
    "org.inviwo.StreamLines",      // Class identifier
    "Stream Lines",                // Display name
    "Vector Field Visualization",  // Category
    CodeState::Experimental,       // Code state
    Tags::CPU,                     // Tags
};
const ProcessorInfo StreamLines::getProcessorInfo() const { return processorInfo_; }

StreamLines::StreamLines()
    : Processor()
    , sampler_("sampler")
    , seedPoints_("seedpoints")
    , linesStripsMesh_("linesStripsMesh_")
    , streamLineProperties_("streamLineProperties", "Stream Line Properties")
    , tf_("transferFunction", "Transfer Function")
    , velocityScale_("velocityScale_", "Velocity Scale (inverse)", 1, 0, 10)
    , maxVelocity_("minMaxVelocity", "Velocity Range", "0", InvalidationLevel::Valid) {
    

    addPort(sampler_);
    addPort(seedPoints_);
    addPort(linesStripsMesh_);

    maxVelocity_.setReadOnly(true);

    addProperty(streamLineProperties_);

    addProperty(tf_);
    addProperty(velocityScale_);
    addProperty(maxVelocity_);

    tf_.get().clearPoints();
    tf_.get().addPoint(vec2(0, 1), vec4(0, 0, 1, 1));
    tf_.get().addPoint(vec2(0.5, 1), vec4(1, 1, 0, 1));
    tf_.get().addPoint(vec2(1, 1), vec4(1, 0, 0, 1));

    setAllPropertiesCurrentStateAsDefault();
}

StreamLines::~StreamLines() {}

void StreamLines::process() {
    auto mesh = std::make_shared<BasicMesh>();
    mesh->setModelMatrix(sampler_.getData()->getModelMatrix());
    mesh->setWorldMatrix(sampler_.getData()->getWorldMatrix());

    auto m = streamLineProperties_.getSeedPointTransformationMatrix(
        sampler_.getData()->getCoordinateTransformer());


    ImageSampler tf(tf_.get().getData());

    float maxVelocity = 0;
    StreamLineTracer tracer(sampler_.getData(), streamLineProperties_);

    std::vector<BasicMesh::Vertex> vertices;

    for (const auto &seeds : seedPoints_) {
        for (auto &p : (*seeds)) {
            vec4 P = m * vec4(p, 1.0f);
            auto line = tracer.traceFrom(P.xyz());

            auto position = line.getPositions().begin();
            auto velocity = line.getMetaData("velocity").begin();

            auto size = line.getPositions().size();
            if (size == 0) continue;

            auto indexBuffer =
                mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::StripAdjacency);

            indexBuffer->add(0);

            for (size_t i = 0; i < size; i++) {
                vec3 pos(*position);
                vec3 v(*velocity);

                float l = glm::length(vec3(*velocity));
                float d = glm::clamp(l / velocityScale_.get(), 0.0f, 1.0f);
                maxVelocity = std::max(maxVelocity, l);
                auto c = vec4(tf.sample(dvec2(d, 0.0)));

                indexBuffer->add(static_cast<std::uint32_t>(vertices.size()));

                vertices.push_back({pos, glm::normalize(v), pos, c});

                position++;
                velocity++;
            }
            indexBuffer->add(static_cast<std::uint32_t>(vertices.size() - 1));
        }
    }

    mesh->addVertices(vertices);

    linesStripsMesh_.setData(mesh);
    maxVelocity_.set(toString(maxVelocity));
}

}  // namespace
