/*
 * Copyright (C) 2012 Sony Mobile Communications AB.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "DASH - lsm303dlh_mag - wrapper"

#include <stdlib.h>
#include <string.h>
#include <cutils/log.h>
#include "sensors_list.h"
#include "sensors_fifo.h"
#include "sensors_id.h"
#include "sensors_wrapper.h"
#include "sensor_util.h"
#include "sensor_xyz.h"

static void magnetometer_data(struct sensor_api_t *s, struct sensor_data_t *sd)
{
	struct wrapper_desc *d = container_of(s, struct wrapper_desc, api);
	sensors_event_t data;

	data.timestamp = get_current_nano_time();
	data.sensor = d->sensor.handle;
	data.version = d->sensor.version;
	data.type = d->sensor.type;
	data.magnetic.status  = sd->status;
	data.magnetic.x = sd->data[AXIS_X] * sd->scale;
	data.magnetic.y = sd->data[AXIS_Y] * sd->scale;
	data.magnetic.z = sd->data[AXIS_Z] * sd->scale;
	sensors_fifo_put(&data);
}

static struct wrapper_desc magnetometer = {
	.sensor = {
		.name       = "ST magnetometer",
		.vendor     = "ST Microelectronic",
		.version    = sizeof(sensors_event_t),
		.handle     = SENSOR_MAGNETIC_FIELD_HANDLE,
		.type       = SENSOR_TYPE_MAGNETIC_FIELD,
		.maxRange   = 8.2,
		.resolution = 0.1,
		.power      = 1,
	},
	.api = {
		.init      = sensors_wrapper_init,
		.activate  = sensors_wrapper_activate,
		.set_delay = sensors_wrapper_set_delay,
		.close     = sensors_wrapper_close,
		.data      = magnetometer_data,
	},
	.access = {
		.match = {
			SENSOR_TYPE_MAGNETIC_FIELD,
		},
		.m_nr = 1,
	},
};

list_constructor(magnetometer_register);
void magnetometer_register()
{
	sensors_list_register(&magnetometer.sensor, &magnetometer.api);
}
