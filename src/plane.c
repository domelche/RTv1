/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: domelche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 13:25:05 by domelche          #+#    #+#             */
/*   Updated: 2018/08/23 13:27:24 by domelche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

t_plane		*ft_planenew(void)
{
	t_plane	*pln;

	pln = ft_smemalloc(sizeof(t_plane), "ft_planenew");
	pln->origin = ft_point3new(0.0, -1000.0, 0.0);
	pln->norm = ft_point3new(0.0, 1.0, 0.0);
	return (pln);
}

int			ft_is_reachable_plane(void *fig, t_point3 origin, t_point3 direct)
{
	(void)fig;
	(void)origin;
	(void)direct;
	return (1);
}

t_point3		ft_collide_plane(void *fig, t_point3 origin, t_point3 direct)
{

//	printf("in ft_collide_plane: (%f, %f, %f) -> (%f, %f, %f);\n",
//		origin.x, origin.y, origin.z, direct.x, direct.y, direct.z);

	t_plane		*pln;
	t_point3		coll;
	double		t;
	double		dev;

	pln = (t_plane *)fig;
	if (!ft_3_vector_cos(direct, pln->norm))
		return (ft_null_point3new());
	dev = ft_3_vector_dot(pln->norm, direct);
	if (!dev)
		return (ft_null_point3new());
	t = (ft_3_vector_dot(pln->norm, pln->origin) -
			ft_3_vector_dot(pln->norm, origin)) / dev;
	coll = ft_3_add_vector(origin, ft_3_vector_scale(direct, t));
	return ((!ft_point3cmp(ft_unitvector3new(origin, coll), direct, 1e-6)) ?
			ft_null_point3new() : coll);
}

t_point3		ft_get_norm_plane(void *fig, t_point3 coll)
{
	(void)coll;
	return (((t_plane *)fig)->norm);
}

char		*ft_parse_plane(char *attr, t_scene *scn)
{
	t_object	*obj;
	t_plane		*pln;

	obj = ft_parse_object(attr);
	obj->ft_collide = ft_collide_plane;
	obj->ft_is_reachable = ft_is_reachable_plane;
	obj->ft_get_norm = ft_get_norm_plane;
	pln = ft_planenew();
	attr = ft_get_curve(attr, '{');
	ft_get_attr_in_scope(attr, "origin:", (void *)(&(pln->origin)), PNT);
	ft_get_attr_in_scope(attr, "norm:", (void *)(&(pln->norm)), PNT);
	if (ft_vector_len(pln->norm) == 0.0)
		pln->norm = ft_point3new(0.0, 1.0, 0.0);
	pln->norm = ft_tounitvector3(pln->norm);
	pln->norm = ft_tounitvector3(ft_3_rotate_vector(pln->norm,
													obj->rotate.x,
													obj->rotate.y,
													obj->rotate.z));
	pln->origin = ft_3_add_vector(pln->origin, obj->translate);
	obj->fig = pln;
	ft_lstpush(&(scn->objs), ft_nodenew((void *)obj, sizeof(obj)));
	return (ft_get_curve(attr, '}'));
}
