//
// Copyright 2012-2013 Ettus Research LLC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <gras/block.hpp>
#include <gras/factory.hpp>
#include <boost/lexical_cast.hpp>
#include <PMC/Containers.hpp>
#include <stdexcept>

#ifdef HAVE_UHD

#include <uhd/usrp/multi_usrp.hpp>

namespace uhd
{
//declare false equality operator to use tune req
PMC_DECL_FALSE_EQUALITY(uhd::tune_request_t)
}

struct UHDControlPort : gras::Block
{
    UHDControlPort(uhd::usrp::multi_usrp::sptr usrp):
        gras::Block("GrEx UHDControlPort")
    {
        _usrp = usrp;
        //register handlers
        this->register_call("set_command_time", &UHDControlPort::set_command_time);
        this->register_call("get_rx_gain", &UHDControlPort::get_rx_gain);
        this->register_call("set_rx_gain", &UHDControlPort::set_rx_gain);
        this->register_call("get_tx_gain", &UHDControlPort::get_tx_gain);
        this->register_call("set_tx_gain", &UHDControlPort::set_tx_gain);
        this->register_call("get_rx_freq", &UHDControlPort::get_rx_freq);
        this->register_call("set_rx_freq", &UHDControlPort::set_rx_freq);
        this->register_call("get_tx_freq", &UHDControlPort::get_tx_freq);
        this->register_call("set_tx_freq", &UHDControlPort::set_tx_freq);

        this->register_call("get_time_source", &UHDControlPort::get_time_source);
        this->register_call("set_time_source", &UHDControlPort::set_time_source);
        this->register_call("get_clock_source", &UHDControlPort::get_clock_source);
        this->register_call("set_clock_source", &UHDControlPort::set_clock_source);

        this->register_call("get_time_now", &UHDControlPort::get_time_now);
        this->register_call("set_time_now", &UHDControlPort::set_time_now);
        this->register_call("get_time_pps", &UHDControlPort::get_time_pps);
        this->register_call("set_time_pps", &UHDControlPort::set_time_pps);
    }

    void work(const InputItems &, const OutputItems &)
    {
        //there is no work, only handlers
    }

    /*******************************************************************
     * The RX gain
     ******************************************************************/
    void set_rx_gain(const double &gain)
    {
        _usrp->set_rx_gain(gain);
    }

    double get_rx_gain(void)
    {
        return _usrp->get_rx_gain();
    }

    /*******************************************************************
     * The TX gain
     ******************************************************************/
    void set_tx_gain(const double &gain)
    {
        _usrp->set_tx_gain(gain);
    }

    double get_tx_gain(void)
    {
        return _usrp->get_tx_gain();
    }

    /*******************************************************************
     * The RX freq
     ******************************************************************/
    void set_rx_freq(const uhd::tune_request_t &tr)
    {
        _usrp->set_rx_freq(tr);
    }

    double get_rx_freq(void)
    {
        return _usrp->get_rx_freq();
    }

    /*******************************************************************
     * The TX freq
     ******************************************************************/
    void set_tx_freq(const uhd::tune_request_t &tr)
    {
        _usrp->set_tx_freq(tr);
    }

    uhd::tune_request_t get_tx_freq(void)
    {
        return _usrp->get_tx_freq();
    }

    /*******************************************************************
     * Command time
     ******************************************************************/
    void set_command_time(const uhd::time_spec_t &time_spec)
    {
        if (time_spec == uhd::time_spec_t(0.0))
            _usrp->clear_command_time();
        else
            _usrp->set_command_time(time_spec);
    }

    /*******************************************************************
     * time PPS
     ******************************************************************/
    uhd::time_spec_t get_time_pps(void)
    {
        return _usrp->get_time_last_pps(0);
    }

    void set_time_pps(const uhd::time_spec_t &time)
    {
        _usrp->set_time_next_pps(time, 0);
    }

    /*******************************************************************
     * time NOW
     ******************************************************************/
    uhd::time_spec_t get_time_now(void)
    {
        return _usrp->get_time_now(0);
    }

    void set_time_now(const uhd::time_spec_t &time)
    {
        _usrp->set_time_now(time, 0);
    }

    /*******************************************************************
     * time source
     ******************************************************************/
    std::string get_time_source(void)
    {
        return _usrp->get_time_source(0);
    }

    void set_time_source(const std::string &source)
    {
        _usrp->set_time_source(source, 0);
    }

    /*******************************************************************
     * clock source
     ******************************************************************/
    std::string get_clock_source(void)
    {
        return _usrp->get_clock_source(0);
    }

    void set_clock_source(const std::string &source)
    {
        _usrp->set_clock_source(source, 0);
    }

    uhd::usrp::multi_usrp::sptr _usrp;
};

gras::Block *make_uhd_control_port(const std::string &addr)
{
    uhd::usrp::multi_usrp::sptr u = uhd::usrp::multi_usrp::make(addr);
    return new UHDControlPort(u);
}

#else //HAVE_UHD

gras::Block *make_uhd_control_port(const std::string &)
{
    throw std::runtime_error("UHDControlPort::make - GrEx not build with UHD support");
}

#endif //HAVE_UHD

GRAS_REGISTER_FACTORY("/ex/uhd_control_port", make_uhd_control_port)
