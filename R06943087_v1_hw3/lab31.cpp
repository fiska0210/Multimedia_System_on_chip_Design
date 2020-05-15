#include "vpi_user.h"
#include <Python.h>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <vector>

using namespace std;

static PyObject *p_cycle_function;
static PyObject *p_bridge_module;
static PyObject *p_test_module;
static vpiHandle v_i_valid;
static vpiHandle v_i_char;
static vpiHandle v_o_valid;
static vpiHandle v_o_char;

extern "C" {


static PyObject* ReadBus(PyObject *dummy, PyObject *args)
{
	unsigned valid, data;
	s_vpi_value read_bus_value; 
	read_bus_value.format = vpiIntVal;
	vpi_get_value(v_o_valid, &read_bus_value);
	valid = read_bus_value.value.integer;

	vpi_get_value(v_o_char, &read_bus_value);
	data = read_bus_value.value.integer;

	return Py_BuildValue("II", valid, data);

}

static PyObject* WriteBus(PyObject *dummy, PyObject *args)
{
	unsigned valid, data;
	s_vpi_value v; //s_vpi_value hold a pointer to s_vpi_vecval
	s_vpi_vecval vecval;
	s_vpi_time tm {vpiSimTime, 0, 0, 0};
	v.format = vpiVectorVal;
	vecval.bval = 0;

    v.value.vector = &vecval;

	if (!PyArg_ParseTuple(args, "II", &valid, &data)) {
		return nullptr;
	}

    vecval.aval = valid;
	vpi_put_value(v_i_valid, &v, &tm, vpiInertialDelay);
    
	vecval.aval = data;
	vpi_put_value(v_i_char, &v, &tm, vpiInertialDelay);

	Py_INCREF(Py_None);
	return Py_None;
}

static PLI_INT32 Cycle(PLI_BYTE8 *dummy)
{
	// TODO: use PyObject_CallFunction to call Cycle (p_cycle_function) correctly
    PyObject *py;
	PyObject_CallFunction(p_cycle_function,"");
	PyErr_Print();
	Py_XDECREF(py);
	return 0;
}

static PyObject* InitBridgeModule()
{
	// Python can link to Verilog throught them throught VPI
	static PyMethodDef bridge_methods[] = {
		{"WriteBus", WriteBus, METH_VARARGS, ""},
		{"ReadBus", ReadBus, METH_VARARGS, ""},
		{nullptr, nullptr, 0, nullptr}
	};
	static PyModuleDef bridge_module = {
		PyModuleDef_HEAD_INIT,
		"lab31_vpi", nullptr, -1,
		bridge_methods,
		nullptr, nullptr, nullptr, nullptr
	};
	p_bridge_module = PyModule_Create(&bridge_module);
	return p_bridge_module;
}

static PLI_INT32 Init(PLI_BYTE8 *dummy)
{
	// Python part
	PyImport_AppendInittab("lab31_vpi", InitBridgeModule);
	Py_Initialize();
	p_test_module = PyImport_ImportModule("lab31_py");
	PyErr_Print();
	p_cycle_function = PyObject_GetAttrString(p_test_module, "Cycle");
	PyErr_Print();
	return 0;
}

static PLI_INT32 Final(PLI_BYTE8 *dummy)
{
	Py_DECREF(p_cycle_function);
	Py_DECREF(p_bridge_module);
	Py_DECREF(p_test_module);
	return 0;
}

void VpiBoot()
{
	// VPI part
	static s_vpi_systf_data tasks[] = {
		{vpiSysTask, vpiSysTask, "$Lab31Cycle", Cycle, nullptr, nullptr, nullptr},
		{vpiSysTask, vpiSysTask, "$Lab31Start", Init, nullptr, nullptr, nullptr},
		{vpiSysTask, vpiSysTask, "$Lab31Stop", Final, nullptr, nullptr, nullptr}
	};
	for (auto&& task: tasks) {
		vpi_register_systf(&task);
	}
	v_i_valid = vpi_handle_by_name("tb.u_to_upper.i_valid", nullptr);
	v_i_char  = vpi_handle_by_name("tb.u_to_upper.i_char" , nullptr);
	// TODO: also linke v_o_valid and v_o_char
    v_o_valid = vpi_handle_by_name("tb.u_to_upper.o_valid", nullptr);
	v_o_char  = vpi_handle_by_name("tb.u_to_upper.o_char" , nullptr);

}

}

void (*vlog_startup_routines[])() = {
	VpiBoot,
	nullptr
};
