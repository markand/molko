#include "alloc.h"
#include "coro.h"

#define MCO_ALLOC(Size) mlk_alloc_new(1, (Size))
#define MCO_DEALLOC(Ptr, Size) mlk_alloc_free((Ptr))

#define MINICORO_IMPL
#include <minicoro.h>

static void
mlk_coro_wrap_entry(struct mco_coro *self)
{
	struct mlk_coro *coro = self->user_data;

	coro->entry(coro);
}

void
mlk_coro_yield(void)
{
	struct mco_coro *coro = mco_running();

	mco_yield(coro);
}

int
mlk_coro_resumable(const struct mlk_coro *coro)
{
	assert(coro);

	return coro->mco_coro && mco_status(coro->mco_coro) == MCO_SUSPENDED;
}

void
mlk_coro_resume(struct mlk_coro *coro)
{
	assert(coro);

	mco_resume(coro->mco_coro);
}

void
mlk_coro_spawn(struct mlk_coro *coro)
{
	assert(coro);
	assert(coro->entry);

	coro->mco_desc = mco_desc_init(mlk_coro_wrap_entry, 0);
	coro->mco_desc.user_data = coro;

	mco_create(&coro->mco_coro, &coro->mco_desc);
	mco_resume(coro->mco_coro);
}
