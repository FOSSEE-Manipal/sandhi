//
// Copyright 2013 Ettus Research LLC
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

#ifdef HAVE_UHD

%{

#include <uhd/types/time_spec.hpp>
#include <uhd/types/tune_request.hpp>

#include <PMC/PMC.hpp>

//declare false equality operator to use tune req
PMC_DECL_FALSE_EQUALITY(uhd::tune_request_t)

%}

%import <uhd_swig.i>

%include <PMC/Registry.i>

DECL_PMC_SWIG_TYPE(uhd::time_spec_t, swig_uhd_time_spec)
DECL_PMC_SWIG_TYPE(uhd::tune_request_t, swig_uhd_tune_request)

%pythoncode %{

from gnuradio import uhd

RegisterPy2PMC(
    is_py = lambda x: isinstance(x, uhd.time_spec_t),
    py2pmc = swig_uhd_time_spec_to_pmc,
)

RegisterPMC2Py(
    is_pmc = pmc_is_swig_uhd_time_spec,
    pmc2py = pmc_to_swig_uhd_time_spec,
)

RegisterPy2PMC(
    is_py = lambda x: isinstance(x, uhd.tune_request_t),
    py2pmc = swig_uhd_tune_request_to_pmc,
)

RegisterPMC2Py(
    is_pmc = pmc_is_swig_uhd_tune_request,
    pmc2py = pmc_to_swig_uhd_tune_request,
)

%}

#endif
