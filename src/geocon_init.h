#pragma once

namespace geocon {

/**
 * Initialize all global GeoCon functionality.
 *
 * Can be called multiple times in a single process. Following calls
 * will not affect state and will return immediately.
 */
bool init();
}
