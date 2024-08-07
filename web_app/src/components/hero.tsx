// Copyright 2024 The Pigweed Authors
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

import React from 'react'
import { useAppState } from '../common/state';
import { StatusData } from '../common/status';
import Alert from '@mui/material/Alert';

export function Hero(){
    const state = useAppState();
    const status = StatusData[state.currentReading.alarmState.aqDescription];
    return (
        <div className='hero-container'>
            <div className='main-status' style={{color: status.color}}>
                <span className="status-subtitle-text">Air Quality: {status.title}</span>
                <div className='status-icon-container'>
                    <span className="material-symbols-outlined icon">
                        {status.icon}
                    </span>
                    <span className='status-icon-text'>{state.basicMode? "NA": `${state.currentReading.score}%`}</span>
                </div>
            </div>
            {state.basicMode && <Alert severity="warning"><b>No Enviro+ Board Found!</b><br/>Showing onboard temperature only. To see all data, connect enviro+ and flash the Production app.</Alert>}
        </div>
        
    )
}