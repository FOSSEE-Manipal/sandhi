// Copyright (C) by Josh Blum. See LICENSE.txt for licensing information.

#include <grextras/opencl_block.hpp>
#include <stdexcept>
#include <boost/format.hpp>
#include <iostream>
#include <utility>
#include <boost/make_shared.hpp>

//http://www.codeproject.com/Articles/92788/Introductory-Tutorial-to-OpenCL
//http://developer.amd.com/tools/heterogeneous-computing/amd-accelerated-parallel-processing-app-sdk/introductory-tutorial-to-opencl/
//http://www.khronos.org/registry/cl/specs/opencl-cplusplus-1.1.pdf

using namespace grextras;

#ifdef HAVE_OPENCL

#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#include <CL/cl.hpp>

inline void checkErr(cl_int err, const char * name)
{
    if (err != CL_SUCCESS) throw std::runtime_error(str(
        boost::format("ERROR: %s (%d)") % name % err));
}

struct OpenClBlockImpl : OpenClBlock
{
    OpenClBlockImpl(const std::string &dev_type):
        gras::Block("GrExtras OpenClBlock")
    {

        /***************************************************************
         * Determine device type
         **************************************************************/
        if (dev_type == "CPU") _cl_dev_type = CL_DEVICE_TYPE_CPU;
        else if (dev_type == "GPU") _cl_dev_type = CL_DEVICE_TYPE_GPU;
        else throw std::runtime_error("OpenClBlock unknown device type: " + dev_type);

        /***************************************************************
         * Enumerate platforms
         **************************************************************/
        {
            cl::Platform::get(&_cl_platforms);
            checkErr(_cl_platforms.size() != 0 ? CL_SUCCESS : -1, "cl::Platform::get");
            std::cerr << "Number of platforms: " << _cl_platforms.size() << ", "
                      << "selecting 0 ..." << std::endl;
            _cl_platform = _cl_platforms[0];
            std::string platformStr;
            _cl_platform.getInfo((cl_platform_info)CL_PLATFORM_NAME, &platformStr);
            std::cerr << "    name: " << platformStr << std::endl;
            _cl_platform.getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformStr);
            std::cerr << "    vendor: " << platformStr << std::endl;
            _cl_platform.getInfo((cl_platform_info)CL_PLATFORM_VERSION, &platformStr);
            std::cerr << "    version: " << platformStr << std::endl;
        }

        /***************************************************************
         * create context
         **************************************************************/
        {
            cl_context_properties cprops[3] =
                {CL_CONTEXT_PLATFORM, (cl_context_properties)(_cl_platform)(), 0};

            cl_int err = CL_SUCCESS;
            _cl_context = cl::Context(
                _cl_dev_type,
                cprops,
                NULL,
                NULL,
                &err
            );
            checkErr(err, "cl::Context");
        }

        /***************************************************************
         * enumerate devices
         **************************************************************/
        {
            _cl_devices = _cl_context.getInfo<CL_CONTEXT_DEVICES>();
            checkErr(_cl_devices.size() > 0 ? CL_SUCCESS : -1, "devices.size() > 0");
        }
    }

    ~OpenClBlockImpl(void)
    {
        //NOP
    }

    void set_program(const std::string &name, const std::string &source, const std::string &options)
    {
        cl::Program::Sources cl_source(1,
            std::make_pair(source.c_str(), source.length()+1));
        _cl_program = cl::Program(_cl_context, cl_source);
        cl_int err = _cl_program.build(_cl_devices, options.c_str());
        checkErr(err, "Program::build");

        _cl_kernel = cl::Kernel(_cl_program, name.c_str(), &err);
        checkErr(err, "Kernel create");
    }

    void notify_topology(const size_t num_inputs, const size_t num_outputs)
    {
        
    }

    void work(const InputItems &ins, const OutputItems &outs)
    {
        
    }

    gras::BufferQueueSptr output_buffer_allocator(
        const size_t which_output,
        const gras::SBufferConfig &config
    ){
        
    }

    gras::BufferQueueSptr input_buffer_allocator(
        const size_t which_input,
        const gras::SBufferConfig &config
    ){
        
    }

    cl_device_type _cl_dev_type;
    cl::Context _cl_context;
    cl::vector<cl::Platform> _cl_platforms;
    cl::Platform _cl_platform;
    cl::vector<cl::Device> _cl_devices;
    cl::Program _cl_program;
    cl::Kernel _cl_kernel;

};

OpenClBlock::sptr OpenClBlock::make(const std::string &dev_type)
{
    return boost::make_shared<OpenClBlockImpl>(dev_type);
}

#else //HAVE_OPENCL

OpenClBlock::sptr OpenClBlock::make(const std::string &)
{
    throw std::runtime_error("OpenClBlock::make sorry, built without Open CL support");
}

#endif //HAVE_OPENCL
