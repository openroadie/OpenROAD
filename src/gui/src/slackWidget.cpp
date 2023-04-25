///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, The Regents of the University of California
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "slackWidget.h"

#include "gui/gui.h"

namespace gui {

SlackWidget::SlackWidget(QWidget* parent)
    : QDockWidget("Slack Histogram", parent), // QDockWidget* parent = new QDockWidget
      bar_set_(new QBarSet("Slack [ns]")),
      series_(new QBarSeries),
      chart_(new QChart),
      values_x_(new QBarCategoryAxis),
      values_y_(new QValueAxis)
{
  setObjectName("slack_histogram"); // for settings

  *bar_set_ << 1 << 2 << 3 << 4 << 5 
            << 6 << 7 << 8 << 9;

  bar_set_->setColor(0x328930);

  series_->append(bar_set_);

  chart_->addSeries(series_);
  chart_->setTitle("End-Point Slack");

  QStringList time_values_;
  time_values_ << "-0.4" << "-0.3" << "-0.2" << "-0.1" << "0"
               << "0.1" << "0.2" << "0.3" << "0.4";

  values_x_->append(time_values_);

  values_y_->setRange(0,10);
  values_y_->setTickCount(10);
  values_y_->setLabelFormat("%i");

  chart_->addAxis(values_x_, Qt::AlignBottom);
  series_->attachAxis(values_x_);
  chart_->addAxis(values_y_, Qt::AlignLeft);
  series_->attachAxis(values_y_);

  chart_->legend()->setVisible(true);
  chart_->legend()->setAlignment(Qt::AlignBottom);

  QChartView *display_ = new QChartView(chart_);
  setWidget(display_);

}

}