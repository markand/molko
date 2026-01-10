#ifndef MLK_CORE_CORO_H
#define MLK_CORE_CORO_H

#include <minicoro.h>

struct mlk_coro {
	/**
	 * (init)
	 *
	 * Entrypoint.
	 */
	void (*entry)(struct mlk_coro *);

	/* private */
	struct mco_coro *mco_coro;
	struct mco_desc  mco_desc;
};

void
mlk_coro_yield(void);

int
mlk_coro_resumable(const struct mlk_coro *coro);

void
mlk_coro_resume(struct mlk_coro *coro);

void
mlk_coro_spawn(struct mlk_coro *coro);

#endif /* !MLK_CORE_CORO_H */
