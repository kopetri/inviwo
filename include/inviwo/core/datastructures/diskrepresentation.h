/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#ifndef IVW_DISKREPRESENTATION_H
#define IVW_DISKREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/cloneableptr.h>
#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

class DiskRepresentationLoader;

class IVW_CORE_API DiskRepresentation {
public:
    DiskRepresentation();
    DiskRepresentation(std::string, DiskRepresentationLoader* loader = nullptr);
    DiskRepresentation(const DiskRepresentation& rhs) = default;
    DiskRepresentation& operator=(const DiskRepresentation& that) = default;
    virtual ~DiskRepresentation() = default;    
    virtual DiskRepresentation* clone() const;

    const std::string& getSourceFile() const;
    bool hasSourceFile() const;

    void setLoader(DiskRepresentationLoader* loader);

    std::shared_ptr<DataRepresentation> createRepresentation() const;
    void updateRepresentation(std::shared_ptr<DataRepresentation> dest) const;

private:
#include <warn/push>
#include <warn/ignore/dll-interface>
    std::string sourceFile_;
#include <warn/pop>

    // DiskRepresentation owns a DataReader to be able to convert it self into RAM.
    util::cloneable_ptr<DiskRepresentationLoader> loader_;
};

class IVW_CORE_API DiskRepresentationLoader {
public:
    virtual ~DiskRepresentationLoader() = default;
    virtual DiskRepresentationLoader* clone() const = 0;
    virtual std::shared_ptr<DataRepresentation> createRepresentation() const = 0;
    virtual void updateRepresentation(std::shared_ptr<DataRepresentation> dest) const = 0;
};

}  // namespace

#endif  // IVW_DISKREPRESENTATION_H
