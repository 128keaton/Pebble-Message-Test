//
//  ViewController.swift
//  PebbleSwift
//
//  Created by Keaton Burleson on 9/7/16.
//  Copyright © 2016 Keaton Burleson. All rights reserved.
//

import UIKit
import PebbleKit
class ViewController: UIViewController, PBPebbleCentralDelegate {
	var uuid: NSUUID? = NSUUID(UUIDString: "8e3bf26f-4a21-4d88-9bcd-64ffef5b9978")

	var connectedWatch: PBWatch? = nil
	@IBOutlet var button: UIButton?

	override func viewDidLoad() {
		// Set UUID of watchapp

		var myAppUUIDbytes: UInt8 = 0
		uuid?.getUUIDBytes(&myAppUUIDbytes)
		PBPebbleCentral.defaultCentral().appUUID = uuid
		PBPebbleCentral.defaultCentral().run()
		PBPebbleCentral.defaultCentral().delegate = self
		print(uuid?.UUIDString)
		super.viewDidLoad()
		// Do any additional setup after loading the view, typically from a nib.
	}

	func pebbleCentral(central: PBPebbleCentral, watchDidConnect watch: PBWatch, isNew: Bool) {
		print("Watch connected: " + watch.name)
		// Keep a reference to this watch

		connectedWatch = watch
		self.connectedWatch?.appMessagesAddReceiveUpdateHandler({ (let watch, let update) -> Bool in

			for key in update.keys {
				if update[key] != nil {
					print("Key: \(key)")
					self.button?.setTitle(update[key] as? String, forState: .Normal)
				}
			}

			return true
		})
	}

	func pebbleCentral(central: PBPebbleCentral, watchDidDisconnect watch: PBWatch) {
		print("Watch disconnected: " + watch.name)
		// check if the watch is our favorite watch :D
		if watch == connectedWatch {
			connectedWatch = nil
		}
	}

	@IBAction func sendTestMessage() {
		let update: Dictionary<NSNumber, AnyObject> = [0: "Spaghetto!"]
		self.button?.setTitle(update[0] as? String, forState: .Normal)
		connectedWatch!.appMessagesPushUpdate(update) { (let watch, let update, let error) in
			if (error == nil) {
				print("Successfully sent message")
			} else {
				print("Encountered an error: " + error.debugDescription)
			}
		}

	}
	override func didReceiveMemoryWarning() {
		super.didReceiveMemoryWarning()
		// Dispose of any resources that can be recreated.
	}

}

