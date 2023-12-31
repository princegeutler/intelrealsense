// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2019 Intel Corporation. All Rights Reserved.

#include "l500-color.h"
#include "l500-private.h"
#include "l500-motion.h"
#include "../backend.h"
#include "proc/motion-transform.h"

#include <rsutils/string/from.h>


namespace librealsense
{

#ifdef _WIN32
        std::vector<std::pair<std::string, stream_profile>> l500_sensor_name_and_hid_profiles =
        {{ "HID Sensor Class Device: Gyroscope",     {  RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_GYRO,  0, 1, 1, 100  }},
         { "HID Sensor Class Device: Gyroscope",     {  RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_GYRO,  0, 1, 1, 200  }},
         { "HID Sensor Class Device: Gyroscope",     {  RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_GYRO,  0, 1, 1, 400  }},
         { "HID Sensor Class Device: Accelerometer", {  RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_ACCEL, 0, 1, 1, 100  }},
         { "HID Sensor Class Device: Accelerometer", {  RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_ACCEL, 0, 1, 1, 200  }},
         { "HID Sensor Class Device: Accelerometer", {  RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_ACCEL, 0, 1, 1, 400  }}};

        // Translate frequency to SENSOR_PROPERTY_CURRENT_REPORT_INTERVAL
        std::map<rs2_stream, std::map<unsigned, unsigned>> l500_fps_and_sampling_frequency_per_rs2_stream =
        { {RS2_STREAM_ACCEL,{{100,  1000},
                             {200,  500},
                             {400,  250}}},
         {RS2_STREAM_GYRO,  {{100,  1000},
                             {200,  500},
                             {400,  250}}}};

#else
    std::vector<std::pair<std::string, stream_profile>> l500_sensor_name_and_hid_profiles =
   {{ "gyro_3d",        {   RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_GYRO,  0, 1, 1, 100   }},
    { "gyro_3d",        {   RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_GYRO,  0, 1, 1, 200   }},
    { "gyro_3d",        {   RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_GYRO,  0, 1, 1, 400   }},
    { "accel_3d",       {   RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_ACCEL, 0, 1, 1, 100   }},
    { "accel_3d",       {   RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_ACCEL, 0, 1, 1, 200   }},
    { "accel_3d",       {   RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_ACCEL, 0, 1, 1, 400   }}};

   // The frequency selector is vendor and model-specific
   std::map<rs2_stream, std::map<unsigned, unsigned>> l500_fps_and_sampling_frequency_per_rs2_stream =
   { {RS2_STREAM_ACCEL,{{100,  1},
                        {200,  2},
                        {400,  4}}},
    {RS2_STREAM_GYRO,  {{100,  1},
                        {200,  2},
                        {400,  4}}} };
#endif

    class l500_hid_sensor : public synthetic_sensor, public motion_sensor
    {
    public:
        explicit l500_hid_sensor(std::string name,
            std::shared_ptr<sensor_base> sensor,
            device* device,
            l500_motion* owner)
            : synthetic_sensor(name, sensor, device), _owner(owner)
        {
        }

        rs2_motion_device_intrinsic get_motion_intrinsics(rs2_stream stream) const
        {
            return _owner->get_motion_intrinsics(stream);
        }

        stream_profiles init_stream_profiles() override
        {
            auto lock = environment::get_instance().get_extrinsics_graph().lock();
            auto results = synthetic_sensor::init_stream_profiles();

            for (auto p : results)
            {
                // Register stream types
                if (p->get_stream_type() == RS2_STREAM_ACCEL)
                    assign_stream(_owner->_accel_stream, p);
                if (p->get_stream_type() == RS2_STREAM_GYRO)
                    assign_stream(_owner->_gyro_stream, p);

                //set motion intrinsics
                if (p->get_stream_type() == RS2_STREAM_ACCEL || p->get_stream_type() == RS2_STREAM_GYRO)
                {
                    auto motion = dynamic_cast<motion_stream_profile_interface*>(p.get());
                    assert(motion); //Expecting to succeed for motion stream (since we are under the "if")
                    auto st = p->get_stream_type();
                    motion->set_intrinsics([this, st]() { return get_motion_intrinsics(st); });
                }
            }

            return results;
        }

    private:
        const l500_motion* _owner;
    };

    std::shared_ptr<synthetic_sensor> l500_motion::create_hid_device(std::shared_ptr<context> ctx, const std::vector<platform::hid_device_info>& all_hid_infos)
    {
        if (all_hid_infos.empty())
        {
            LOG_WARNING("No HID info provided, IMU is disabled");
            return nullptr;
        }

        std::unique_ptr<frame_timestamp_reader> iio_hid_ts_reader(new iio_hid_timestamp_reader());
        std::unique_ptr<frame_timestamp_reader> custom_hid_ts_reader(new iio_hid_timestamp_reader());
        auto enable_global_time_option = std::shared_ptr<global_time_option>(new global_time_option());
        auto raw_hid_ep = std::make_shared<hid_sensor>(ctx->get_backend().create_hid_device(all_hid_infos.front()),
            std::unique_ptr<frame_timestamp_reader>(new global_timestamp_reader(std::move(iio_hid_ts_reader), _tf_keeper, enable_global_time_option)),
            std::unique_ptr<frame_timestamp_reader>(new global_timestamp_reader(std::move(custom_hid_ts_reader), _tf_keeper, enable_global_time_option)),
            l500_fps_and_sampling_frequency_per_rs2_stream,
            l500_sensor_name_and_hid_profiles,
            this);

        auto hid_ep = std::make_shared<l500_hid_sensor>("Motion Module", raw_hid_ep, this, this);

        hid_ep->register_option(RS2_OPTION_GLOBAL_TIME_ENABLED, enable_global_time_option);
        hid_ep->get_option(RS2_OPTION_GLOBAL_TIME_ENABLED).set(0);
        hid_ep->register_option(RS2_OPTION_GLOBAL_TIME_ENABLED, enable_global_time_option);

        // register pre-processing
        std::shared_ptr<enable_motion_correction> mm_correct_opt = nullptr;

        //  Motion intrinsic calibration presents is a prerequisite for motion correction.
        try
        {
            // L515 motion correction with IMU supported from FW version 01.04.01.00
            if (_fw_version >= firmware_version("1.4.1.0") && _mm_calib)
            {
                mm_correct_opt = std::make_shared<enable_motion_correction>(hid_ep.get(), option_range{ 0, 1, 1, 1 });
                hid_ep->register_option(RS2_OPTION_ENABLE_MOTION_CORRECTION, mm_correct_opt);
            }
        }
        catch (...) {}

        hid_ep->register_processing_block(
            { {RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_ACCEL} },
            { {RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_ACCEL} },
            [&, mm_correct_opt]() { return std::make_shared<acceleration_transform>(_mm_calib, mm_correct_opt); }
        );

        hid_ep->register_processing_block(
            { {RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_GYRO} },
            { {RS2_FORMAT_MOTION_XYZ32F, RS2_STREAM_GYRO} },
            [&, mm_correct_opt]() { return std::make_shared<gyroscope_transform>(_mm_calib, mm_correct_opt); }
        );
        return hid_ep;
    }

    l500_motion::l500_motion(std::shared_ptr<context> ctx, const platform::backend_device_group & group)
        :device(ctx, group), l500_device(ctx, group), 
          _accel_stream(new stream(RS2_STREAM_ACCEL)),
         _gyro_stream(new stream(RS2_STREAM_GYRO))
    {
        std::vector<platform::hid_device_info> hid_infos = group.hid_devices;

        if (!hid_infos.empty())
        {
            // product id
            _pid = static_cast<uint16_t>(strtoul(hid_infos.front().pid.data(), nullptr, 16));

            // motion correction
            _mm_calib = std::make_shared<mm_calib_handler>(_hw_monitor, _pid);
            _accel_intrinsic = std::make_shared<lazy<ds::imu_intrinsic>>([this]() { return _mm_calib->get_intrinsic(RS2_STREAM_ACCEL); });
            _gyro_intrinsic = std::make_shared<lazy<ds::imu_intrinsic>>([this]() { return _mm_calib->get_intrinsic(RS2_STREAM_GYRO); });

            // use predefined extrinsics
            _depth_to_imu = std::make_shared<lazy<rs2_extrinsics>>([this]() { return _mm_calib->get_extrinsic(RS2_STREAM_ACCEL); });
        }

        // Make sure all MM streams are positioned with the same extrinsics
        environment::get_instance().get_extrinsics_graph().register_extrinsics(*_depth_stream, *_accel_stream, _depth_to_imu);
        environment::get_instance().get_extrinsics_graph().register_same_extrinsics(*_accel_stream, *_gyro_stream);
        register_stream_to_extrinsic_group(*_gyro_stream, 0);
        register_stream_to_extrinsic_group(*_accel_stream, 0);

        auto hid_ep = create_hid_device(ctx, group.hid_devices);
        if (hid_ep)
        {
            _motion_module_device_idx = add_sensor(hid_ep);
            // HID metadata attributes
            hid_ep->get_raw_sensor()->register_metadata(RS2_FRAME_METADATA_FRAME_TIMESTAMP, make_hid_header_parser(&platform::hid_header::timestamp));
        }
    }

    std::vector<tagged_profile> l500_motion::get_profiles_tags() const
    {
        std::vector<tagged_profile> tags;

        tags.push_back({ RS2_STREAM_GYRO, -1, 0, 0, RS2_FORMAT_MOTION_XYZ32F, 200, profile_tag::PROFILE_TAG_SUPERSET | profile_tag::PROFILE_TAG_DEFAULT });
        tags.push_back({ RS2_STREAM_ACCEL, -1, 0, 0, RS2_FORMAT_MOTION_XYZ32F, 200, profile_tag::PROFILE_TAG_SUPERSET | profile_tag::PROFILE_TAG_DEFAULT });
        return tags;
    }

    rs2_motion_device_intrinsic l500_motion::get_motion_intrinsics(rs2_stream stream) const
    {
        if (stream == RS2_STREAM_ACCEL)
            return create_motion_intrinsics(**_accel_intrinsic);

        if (stream == RS2_STREAM_GYRO)
            return create_motion_intrinsics(**_gyro_intrinsic);

        throw std::runtime_error( rsutils::string::from()
                                  << "Motion Intrinsics unknown for stream " << rs2_stream_to_string( stream ) << "!" );

    }
}
