import * as React from "react";
import * as ReactDOM from "react-dom";
import * as IDB from "idb";

import { Hello } from "./components/Hello";

ReactDOM.render(
	<Hello compiler="TypeScript" framework="React" />,
	document.getElementById("example")
);

async function idbTest() {
	const db = await IDB.default.open("test", 1, db => {
		switch(db.oldVersion)
		{
			case 0:
			db.createObjectStore("objectStore", {keyPath: "key"})
		}
	})
	try {
		const tx = db.transaction("objectStore", "readwrite")
		const store = tx.objectStore("objectStore")
		const valid = await store.put({key: "foo", value: "hello world"})
		const items = await store.getAll()
		await tx.complete
		console.log(items)
	}
	finally {
		db.close()
	}
}

idbTest()
