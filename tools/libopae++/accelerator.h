// Copyright(c) 2017, Intel Corporation
//
// Redistribution  and  use  in source  and  binary  forms,  with  or  without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of  source code  must retain the  above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name  of Intel Corporation  nor the names of its contributors
//   may be used to  endorse or promote  products derived  from this  software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
// IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT OWNER  OR CONTRIBUTORS BE
// LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
// CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT LIMITED  TO,  PROCUREMENT  OF
// SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,  DATA, OR PROFITS;  OR BUSINESS
// INTERRUPTION)  HOWEVER CAUSED  AND ON ANY THEORY  OF LIABILITY,  WHETHER IN
// CONTRACT,  STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE  OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <memory>
#include <opae/fpga.h>
#include "option_map.h"
#include "fpga_resource.h"
#include "log.h"
#include "dma_buffer.h"
#include "perf_counters.h"
#include "mmio.h"

namespace intel
{
namespace fpga
{

class accelerator : public fpga_resource, public mmio
{
public:
    virtual ~accelerator(){}

    enum status_t
    {
        unknown = 0,
        opened,
        released
    };

    fpga_resource::type_t type();

    virtual bool open(bool shared);

    virtual bool close();

    typedef std::shared_ptr<accelerator> ptr_t;

    static std::vector<ptr_t> enumerate(std::vector<intel::utils::option_map::ptr_t> options);

    virtual bool write_mmio32(uint32_t offset, uint32_t value);

    virtual bool write_mmio64(uint32_t offset, uint64_t value);

    virtual bool read_mmio32(uint32_t offset, uint32_t & value);

    virtual bool read_mmio64(uint32_t offset, uint64_t & value);

    virtual bool reset();

    virtual bool ready();

    virtual void release();

    virtual dma_buffer::ptr_t allocate_buffer(std::size_t size);

    virtual uint64_t umsg_num();

    virtual bool umsg_set_mask(uint64_t mask);

    virtual uint64_t * umsg_get_ptr();

    fpga_cache_counters cache_counters() const;

    fpga_fabric_counters fabric_counters() const;

protected:
    accelerator(const accelerator & other);
    accelerator & operator=(const accelerator & other);

private:

    accelerator(shared_token token, fpga_properties props,
         const std::string &par_sysfs, fpga_resource::ptr_t parent);

    status_t status_;
    std::string parent_sysfs_;
};

} // end of namespace fpga
} // end of namespace intel
