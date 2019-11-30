// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class SceneLayerMessageFunction : public MessageFunc {
	public:
		void AcceptMessage(const Message& message) override;
	};

}
