namespace Assistant {
	interface State {
		transactions: Map<TransactionID, Transaction>
		categories: Map<CategoryID, Category>
		payees: Map<PayeeID, PayeeID>
		accounts: Map<AccountID, Account>
	}

	type TransactionID = number
	interface Transaction {
		category: CategoryID
		payee: PayeeID
		account: AccountID
		currency: CurrencyID
		amount: number
		accountCurrencyAmount?: number
	}

	type CategoryID = number
	interface Category {
		name: string
		parent: CategoryID
	}

	type PayeeID = number
	interface Payee {
		name: string
	}

	type AccountID = number
	interface Account {
		name: string
		currency: CurrencyID
	}

	type CurrencyID = number
	interface Currency {
		name: string
		usdFactor: number,
	}
}
