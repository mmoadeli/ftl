/*
 * Copyright (c) 2013 Björn Aili
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */
#ifndef FTL_MAYBET_TESTS_H
#define FTL_MAYBET_TESTS_H

#include <ftl/maybe_trans.h>
#include "base.h"

test_set maybet_tests{
	std::string("maybe_trans"),
	{
		std::make_tuple(
			std::string("functor::map[value]"),
			std::function<bool()>([]() -> bool {
				using ftl::operator%;
				using ftl::function;
				using ftl::value;
				using mf = ftl::maybeT<function<int,int>>;

				auto f = ftl::applicative<mf>::pure(1);
				auto g = [](int x){ return float(x)/4.f; } % f;

				return (*g)(3) == value(0.25f);
			})
		),
		std::make_tuple(
			std::string("functor::map[nothing]"),
			std::function<bool()>([]() -> bool {
				using ftl::operator%;
				using ftl::function;
				using ftl::value;
				using mf = ftl::maybeT<function<int,int>>;

				mf f{ftl::inplace_tag(), [](int){ return ftl::maybe<int>{}; }};
				auto g = [](int x){ return float(x)/4.f; } % f;

				return (*g)(3) == ftl::maybe<int>{};
			})
		),
		std::make_tuple(
			std::string("applicative::pure"),
			std::function<bool()>([]() -> bool {
				using ftl::function;
				using mf = ftl::maybeT<function<int,int>>;

				auto f = ftl::applicative<mf>::pure(10);

				return (*f)(50) == ftl::value(10);
			})
		),
		std::make_tuple(
			std::string("applicative::apply[value,value]"),
			std::function<bool()>([]() -> bool {
				using ftl::function;
				using mf = ftl::maybeT<function<int,int>>;
				using ftl::operator%;
				using ftl::operator*;

				ftl::function<int,int,int> f = [](int x, int y){ return x+y; };
				mf x{ftl::inplace_tag(), [](int x){ return ftl::value(2*x); }};
				mf y{ftl::inplace_tag(), [](int x){ return ftl::value(x/2); }};

				auto z = f % x * y;

				return (*z)(6) == ftl::value(15);
			})
		),
		std::make_tuple(
			std::string("applicative::apply[nothing,value]"),
			std::function<bool()>([]() -> bool {
				using ftl::function;
				using mf = ftl::maybeT<function<int,int>>;
				using ftl::operator%;
				using ftl::operator*;

				ftl::function<int,int,int> f = [](int x, int y){ return x+y; };
				mf x{ftl::inplace_tag(), [](int){ return ftl::maybe<int>{}; }};
				mf y{ftl::inplace_tag(), [](int x){ return ftl::value(x/2); }};

				auto z = f % x * y;

				return (*z)(6) == ftl::nothing;
			})
		),
		std::make_tuple(
			std::string("applicative::apply[value,nothing]"),
			std::function<bool()>([]() -> bool {
				using ftl::function;
				using mf = ftl::maybeT<function<int,int>>;
				using ftl::operator%;
				using ftl::operator*;

				ftl::function<int,int,int> f = [](int x, int y){ return x+y; };
				mf x{ftl::inplace_tag(), [](int x){ return ftl::value(2*x); }};
				mf y{ftl::inplace_tag(), [](int){ return ftl::maybe<int>{}; }};

				auto z = f % x * y;

				return (*z)(6) == ftl::nothing;
			})
		),
		std::make_tuple(
			std::string("applicative::apply[nothing,nothing]"),
			std::function<bool()>([]() -> bool {
				using ftl::function;
				using mf = ftl::maybeT<function<int,int>>;
				using ftl::operator%;
				using ftl::operator*;

				ftl::function<int,int,int> f = [](int x, int y){ return x+y; };
				mf x{ftl::inplace_tag(), [](int){ return ftl::maybe<int>{}; }};
				mf y{ftl::inplace_tag(), [](int){ return ftl::maybe<int>{}; }};

				auto z = f % x * y;

				return (*z)(6) == ftl::nothing;
			})
		),
		std::make_tuple(
			std::string("monad::bind[value,->value]"),
			std::function<bool()>([]() -> bool {
				using ftl::function;
				using mf = ftl::maybeT<function<int,int>>;
				using ftl::operator>>=;

				mf f{ftl::inplace_tag(), [](int x){ return ftl::value(x); }};
				auto g = f >>= [](int x){
					return ftl::maybeT<function<float,int>>{
						ftl::inplace_tag(),
						[x](int y){ return ftl::value(float(x+y)/4.f); }
					};
				};
				return (*g)(2) == 1.f;
			})
		),
		std::make_tuple(
			std::string("monad::bind[nothing,->value]"),
			std::function<bool()>([]() -> bool {
				using ftl::function;
				using mf = ftl::maybeT<function<int,int>>;
				using ftl::operator>>=;

				mf f{ftl::inplace_tag(), [](int){ return ftl::maybe<int>{}; }};
				auto g = f >>= [](int x){
					return ftl::maybeT<function<float,int>>{
						ftl::inplace_tag(),
						[x](int y){ return ftl::value(float(x+y)/4.f); }
					};
				};
				return (*g)(2) == ftl::nothing;
			})
		),
		std::make_tuple(
			std::string("monad::bind[value,->nothing]"),
			std::function<bool()>([]() -> bool {
				using ftl::function;
				using mf = ftl::maybeT<function<int,int>>;
				using ftl::operator>>=;

				mf f{ftl::inplace_tag(), [](int x){ return ftl::value(x); }};
				auto g = f >>= [](int){
					return ftl::maybeT<function<float,int>>{
						ftl::inplace_tag(),
						[](int){ return ftl::maybe<float>{}; }
					};
				};
				return (*g)(2) == ftl::nothing;
			})
		),
		std::make_tuple(
			std::string("monad::bind[nothing,->nothing]"),
			std::function<bool()>([]() -> bool {
				using ftl::function;
				using mf = ftl::maybeT<function<int,int>>;
				using ftl::operator>>=;

				mf f{ftl::inplace_tag(), [](int){ return ftl::maybe<int>{}; }};
				auto g = f >>= [](int){
					return ftl::maybeT<function<float,int>>{
						ftl::inplace_tag(),
						[](int){ return ftl::maybe<float>{}; }
					};
				};
				return (*g)(2) == ftl::nothing;
			})
		)
	}
};

#endif



