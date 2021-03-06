/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paraboloid.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaueur <chaueur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 15:54:34 by chaueur           #+#    #+#             */
/*   Updated: 2018/01/05 18:39:10 by chaueur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "geo.h"
#include "ray.h"
#include "rt.h"
#include "utils.h"

t_geo						*new_paraboloid(t_vec3 position, \
double a, double b)
{
	t_paraboloid	*pb;
	t_geo			*geo;

	geo = NULL;
	if (!malloc_geo((void **)(&pb), sizeof(t_paraboloid), 6, &geo))
		return (0);
	vec3_set(position.x, position.y, position.z, geo->origin);
	pb->facta = a;
	pb->factb = b;
	pb->height = 1.0;
	return (geo);
}

int							belong_to_paraboloid(t_geo *geo, t_vec3 pos)
{
	t_paraboloid		*para;
	double				res;

	para = (t_paraboloid*)geo->curr;
	res = pow((pos.x / para->facta), 2) - pow((pos.y / para->factb), 2) - \
		(pos.z / para->height);
	if (res == 0.0)
		return (1);
	return (0);
}

t_vec3						para_norm(t_geo *geo, t_vec3 hp)
{
	t_paraboloid	*para;
	t_vec3			normal;

	para = (t_paraboloid*)geo;
	normal = vec3_stack(2 * hp.x / pow(para->facta, 2), -2 * hp.y / \
			pow(para->factb, 2), -1 / para->height);
	normal = vec3_normalize_stack(normal);
	return (normal);
}

static void					fill_hp(double *abcd, t_paraboloid *para, t_ray r\
	, t_hp *hp)
{
	hp->t = -1;
	hp->t = positive_smallest((-abcd[1] - sqrt(abcd[3])) / (2 * abcd[0]), \
	(-abcd[1] + sqrt(abcd[3])) / (2 * abcd[0]));
	hp->p = point_at_parameter(hp->t, &r);
	hp->normal = para_norm((t_geo*)para, hp->p);
}

t_hp						hit_paraboloid(t_geo *geo, t_ray *r)
{
	t_paraboloid		*para;
	t_hp				hp;
	t_vec3				dir;
	t_vec3				orig;
	double				abcd[4];

	para = (t_paraboloid *)geo->curr;
	dir = r->dir;
	orig = r->origin;
	hp.t = -1;
	if (para->facta == 0 || para->factb == 0 || para->height == 0)
		return (hp);
	abcd[0] = pow((dir.x / para->facta), 2) - pow((dir.y / para->factb), 2);
	abcd[1] = (2 * orig.x * dir.x / pow(para->facta, 2)) - (2 * orig.y * dir.y \
			/ pow(para->factb, 2)) - (dir.z / para->height);
	abcd[2] = pow((orig.x / para->facta), 2) - pow((orig.y / para->factb), 2) \
	- (orig.z / para->height);
	abcd[3] = pow(abcd[1], 2) - 4 * abcd[0] * abcd[2];
	if (abcd[3] >= 0)
		fill_hp(abcd, para, *r, &hp);
	return (hp);
}
