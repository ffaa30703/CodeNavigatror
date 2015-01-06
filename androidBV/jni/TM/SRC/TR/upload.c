// $Id: get_param_names.c 12 2011-02-18 04:05:43Z cedric.shih@gmail.com $
/*
 * Copyright (C) 2010 AXIM Communications Inc.
 * Copyright (C) 2010 Cedric Shih <cedric.shih@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdlib.h>

#include "log.h"

#include "upload.h"

struct evcpe_upload *evcpe_upload_new(void)
{
	struct evcpe_upload *method;
	evcpe_debug(__func__, "constructing evcpe_upload");
	if (!(method = calloc(1, sizeof(struct evcpe_upload)))) {
		evcpe_error(__func__, "failed to calloc evcpe_upload");
		return NULL;
	}
	return method;
}

void evcpe_upload_free(struct evcpe_upload *method)
{
	if (!method) return;
	evcpe_debug(__func__, "destructing evcpe_upload");
	free(method);
}

struct evcpe_upload_response *evcpe_upload_response_new(void)
{
	struct evcpe_upload_response *method;
	evcpe_debug(__func__, "constructing evcpe_upload_response");
	if (!(method = calloc(1, sizeof(struct evcpe_upload_response)))) {
		evcpe_error(__func__, "failed to calloc evcpe_upload");
		return NULL;
	}
	return method;
}

void evcpe_upload_response_free(
		struct evcpe_upload_response *method)
{
	if (!method) return;
	evcpe_debug(__func__, "destructing evcpe_upload_response");
	free(method);
}


