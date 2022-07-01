#include "OrderBook.h"

void OrderBook::add(int price, int amount, bool bid)
{
	if (bid)
	{
		bids[price] += amount;
	}
	else
	{
		asks[price] += amount;
	}
}

void OrderBook::remove(int price, int amount, bool bid)
{
	auto& table = bid ? bids : asks;
	auto it = table.find(price);

	if (it != table.end())
	{
		it->second -= amount;

		if (it->second == 0)
		{
			table.erase(it);
		}
	}
}

inline bool OrderBook::is_empty() const
{
	return bids.empty() && asks.empty();
}

void OrderBook::add_bid(int price, int amount)
{
	add(price, amount, true);
}

void OrderBook::add_ask(int price, int amount)
{
	add(price, amount, false);
}

void OrderBook::remove_bid(int price, int amount)
{
	remove(price, amount, true);
}

void OrderBook::remove_ask(int price, int amount)
{
	remove(price, amount, false);
}

OrderBook::BidAsk OrderBook::get_bid_ask() const
{
	BidAsk result;

	auto bestBid = bids.rbegin();
	if (bestBid != bids.rend())
	{
		result.bid = *bestBid;
	}

	auto bestAsk = asks.begin();
	if (bestAsk != asks.end())
	{
		result.ask = *bestAsk;
	}

	return result;
}

std::ostream& operator<<(std::ostream& os, const OrderBook& book)
{
	if (book.is_empty())
	{
		os << "OrderBook is Empty";
		return os;
	}

	// Print Asks.
	for (auto iterator = book.asks.rbegin(); iterator != book.asks.rend(); iterator++)
	{
		os << iterator->first << "\t" << iterator->second << std::endl;
	}

	// Print Bids.
	for (auto iterator = book.bids.rbegin(); iterator != book.bids.rend(); iterator++)
	{
		os << iterator->first << "\t" << iterator->second << std::endl;
	}
}

std::ostream& operator<<(std::ostream& os, const OrderBook::BidAsk& ba)
{
	auto print = [&](const OrderBook::BidAsk::Entry& e, const std::string& text)
	{
		bool have_value = e.is_initialized();
		if (have_value)
		{
			auto value = e.get();
			os << value.second << text << "s @" << value.first;
		}
		else
		{
			os << "No " << text;
		}
	};
	print(ba.bid, "bid");
	os << ", ";
	print(ba.ask, "ask");
	return os;
}

boost::optional<int> OrderBook::BidAsk::spread() const
{
	boost::optional<int> result;

	if (bid.is_initialized() && ask.is_initialized())
	{
		result = ask.get().first - bid.get().first;
	}

	return result;
}
